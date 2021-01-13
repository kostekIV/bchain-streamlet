#include <algorithm>
#include <iostream>

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
        for (const hash_t& h: edges.at(node)) {
            if (visited.find(h) != visited.end() or not dfs(h, visited, edges)) {
                return false;
            }
        }
        return true;
    }
}

TreeUnion::TreeUnion(const Tree &tree):
    rootEpoch(-1) {
    insertTree(tree);
    LOG(DEBUG) << "TreeUnion initialised";
}

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
    // finalised nodes can have only one outgoing edge with finalised block
    // plus only one of them must have none such edges.
    return std::all_of(nodes.begin(), nodes.end(), [this](hash_t t) {
        return nodesStatus.at(t) != Status::FINALIZED
            or std::count_if(edges.at(t).begin(), edges.at(t).end(), [this](hash_t outgoing) {
                return nodesStatus.at(outgoing) == Status::FINALIZED;
            }) <= 1;
    }) and countFinalisedBlocksAtEnd() == 1;
}

unsigned TreeUnion::countFinalisedBlocksAtEnd() const {
    int counter = 0;

    auto countUnfinalised = [this] (hash_t node) {
        return std::count_if(edges.at(node).begin(), edges.at(node).end(), [this](hash_t child) {
            return nodesStatus.at(child) != Status::FINALIZED;
        });
    };

    for (auto& node: nodes) {
        if (nodesStatus.at(node) == Status::FINALIZED and countUnfinalised(node) == edges.at(node).size()) {
            counter += 1;
        }
    }
    return counter;
}

void TreeUnion::safeInsertEdge(const hash_t& v, const hash_t& u) {
    safeInitialiseEdges(v);
    edges[v].emplace(u);
}

void TreeUnion::safeInitialiseEdges(const hash_t& v) {
    if (edges.find(v) == edges.end()) {
        edges.emplace(v, std::unordered_set<hash_t>());
    }
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

void TreeUnion::print() const {
    std::cout << "nodes:" << std::endl;
    for (auto x: nodes) {
        std::cout << "* " << x << std::endl;
    }
    std::cout << std::endl;
    std::cout << "edges:" << std::endl;

    for (auto x: edges) {
        std::cout << "* " << x.first << " - [ ";
        for (auto z: x.second) {
            std::cout << z << " ";
        }
        std::cout << "]" << std::endl;
    }
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
