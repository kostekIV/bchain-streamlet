#include <algorithm>

#include "logging/easylogging++.h"

#include "state/block.hpp"
#include "state/tree/tree_union.hpp"

namespace {
    void promoteStatus(const hash_t& key, std::unordered_map<hash_t, Status>& statuses, Status newStasus) {
        if (newStasus == Status::FINALIZED) {
            statuses[key] = newStasus;
        }
    }

    bool dfs(const hash_t& node, std::unordered_set<hash_t>& visited,
             const std::unordered_map<hash_t, std::unordered_set<hash_t>>& edges) {
        visited.emplace(node);
        return std::all_of(edges.at(node).begin(), edges.at(node).end(), [&](const hash_t& h) {
            return visited.find(h) == visited.end() and dfs(h, visited, edges);
        });
    }
}

TreeUnion::TreeUnion():
    rootEpoch(-1) {}

void TreeUnion::insertTree(const Tree& tree) {
    try {
        LOG(DEBUG) << "Populating union with new tree";
        for (const auto&[key, val] : tree.hvMapping) {
            const Block& b = dynamic_cast<const Block&>(val.getContent());
            nodes.emplace(key);
            nodesEpoch.emplace(key, b.epoch);
            auto ret = nodesStatus.emplace(key, val.getStatus());
            
            if (not ret.second) {
                promoteStatus(key, nodesStatus, val.getStatus());
            }

            // inverse edges

            safeInitialiseEdges(val.getContent().hash());
            if (not tree.isRoot(val)) {
                safeInsertEdge(val.getParent().getContent().hash(), val.getContent().hash());
            } else if (b.epoch < rootEpoch) {
                LOG(DEBUG) << "Setting new root with epoch " << b.epoch;
                rootEpoch = b.epoch;
                root = b.hash();
            }
        }
    } catch (const std::bad_cast& e) {
        LOG(ERROR) << "Bad cast - Tree with unsuported type of hashable inside Vertex";
        throw e;
    }
}

TreeUnion& TreeUnion::add(const Tree &tree) {
    insertTree(tree);
    return *this;
}

bool TreeUnion::isValidTree() const {
    std::unordered_set<hash_t> visited;

    return dfs(root, visited, edges) and visited.size() == nodes.size();
}

bool TreeUnion::isConnected() const {
    std::unordered_set<hash_t> visited;
    dfs(root, visited, edges);

    return visited.size() == nodes.size();
}

bool TreeUnion::isFinalizedCorrectly() const {
    long finalized = std::count_if(nodes.begin(), nodes.end(), [this](const hash_t& node) {
        return nodesStatus.at(node) == Status::FINALIZED;
    });

    hash_t current = root;
    hash_t next = root;

    while (true) {
        finalized -= 1;
        for (hash_t child: edges.at(current)) {
            if (nodesStatus.at(child) == Status::FINALIZED) {
                next = child;
                break;
            }
        }
        if (next == current) {
            break;
        }
        current = next;
    }

    return finalized == 0;
}

void TreeUnion::safeInsertEdge(const hash_t& v, const hash_t& u) {
    safeInitialiseEdges(v);
    edges[v].emplace(u);
}

void TreeUnion::safeInitialiseEdges(const hash_t& v) {
    edges.try_emplace(v, std::unordered_set<hash_t>());
}

bool TreeUnion::operator==(TreeUnion const& rhs) const {
    return root == rhs.root
        and rootEpoch == rhs.rootEpoch
        and nodes == rhs.nodes
        and nodesEpoch == rhs.nodesEpoch
        and edges == rhs.edges;
}

bool TreeUnion::operator!=(TreeUnion const& rhs) const {
    return not (*this == rhs);
}

std::unordered_map<hash_t, const Vertex> TreeUnion::asHashVertexMapping() const {
    LOG_IF(not isValidTree(), WARNING) << "Union of trees does not produce valid tree";
    std::unordered_map<hash_t, const Vertex> mapping;

    mapping.emplace(root, Block("", nodesEpoch.at(root), ""));
    mapping.at(root).notarize();
    mapping.at(root).finalize();

    for (const auto&[node, nodeChildren] : edges) {
        for (const auto& child: nodeChildren) {
            mapping.emplace(child, Block(node, nodesEpoch.at(child), ""));
            auto childStatus = nodesStatus.at(child);
            if (childStatus != Status::PRESENT) {
                mapping.at(child).notarize();
            }
            if (childStatus == Status::FINALIZED) {
                mapping.at(child).finalize();
            }
        }
    }
    return mapping;
}
