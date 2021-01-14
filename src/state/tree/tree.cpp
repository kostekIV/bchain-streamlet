#include <algorithm>
#include <utility>

#include "logging/easylogging++.h"

#include "state/tree/tree-test.hpp"

#define range(V) (V).begin(), (V).end()

namespace {
    void checkAndLogError(const std::unordered_map<hash_t, const Vertex>& hvMapping, const hash_t& key) {
        if (hvMapping.find(key) == hvMapping.end()) {
            LOG(ERROR) << "No key " << key << "in tree map";
        }
    }
}

Tree::Tree(const Hashable& rootContent, pred_t finalizationPredicate) :
        finalizationPredicate(std::move(finalizationPredicate)),
        rootHash(std::move(rootContent.hash())),
        hvMapping{{rootHash, rootContent}},
        deepestNotarized(hvMapping.at(rootHash)),
        deepestFinalized(deepestNotarized) {
    auto& root = hvMapping.at(rootHash);
    root.notarize();
    root.finalize();

    LOG(DEBUG) << "[TREE]: " << "Initialised with \"" << root.getContent().hash() << "\" as root hash";
}

bool Tree::isRoot(const Vertex& v) { return v.getDepth() == 0; }

const Hashable& Tree::getSomeDeepestNotarized() const {
    LOG(DEBUG) << "[TREE]: " << "getSomeDeepestNotarized";
    return deepestNotarized.get().getContent();
}

void Tree::addBelow(const Hashable& parent, const Hashable& child) { addBelow(parent.hash(), child); }

void Tree::addBelow(const hash_t& parentHash, const Hashable& child) {
    LOG(DEBUG) << "[TREE]: " << "addBelow parent with hash " << parentHash << " child with hash " << child.hash();
    checkAndLogError(hvMapping, parentHash);
    const Vertex& parentVertex = hvMapping.at(parentHash);
    hash_t childHash = child.hash();
    hvMapping.try_emplace(childHash, child, parentVertex);
}

void Tree::notarize(const Hashable& hashable) {
    LOG(DEBUG) << "[TREE]: " << "notarize block with hash " << hashable.hash();
    checkAndLogError(hvMapping, hashable.hash());
    const Vertex& v = hvMapping.at(hashable.hash());
    v.notarize();
    if (v.getDepth() > deepestNotarized.get().getDepth())
        deepestNotarized = v;
    tryFinalizeUntil(v);
}

void Tree::tryFinalizeUntil(const Vertex& v) {
    LOG(DEBUG) << "[TREE]: " << "tryFinalizeUntil vertex with hash " << v.getContent().hash();
    auto path = getPathFromRootTo(v);
    hashables_t hashables;
    std::for_each(range(path), [&hashables](auto v) { hashables.push_back(std::cref(v.get().getContent())); });
    if (finalizationPredicate(hashables)) {
        for (int i = 0; i < path.size() - 1; ++i)
            path[i].get().finalize();
        deepestFinalized = *(path.end() - 2);
    }
}

Tree::vertices_t Tree::getPathFromRootTo(const Vertex& v) {
    auto current = std::cref(v);
    vertices_t path = {current};
    while (not isRoot(current)) {
        current = current.get().getParent();
        path.push_back(current);
    }
    std::reverse(range(path));
    return path;
}

Tree::hashables_t Tree::getFinalizedChain() const {
    auto finalized = getPathFromRootTo(deepestFinalized);
    hashables_t result;
    std::for_each(range(finalized), [&result](auto v) { result.push_back(v.get().getContent()); });
    return result;
}

bool Tree::isDeepestNotarized(const Hashable& hashable) const { return isDeepestNotarized(hashable.hash()); }

bool Tree::isDeepestNotarized(const hash_t& hash) const {
    LOG(DEBUG) << "[TREE]: " << "check whether " << hash << " is one of the deepest notarized";
    checkAndLogError(hvMapping, hash);
    const auto& v = hvMapping.at(hash);
    return v.getStatus() != Status::PRESENT and v.getDepth() == deepestNotarized.get().getDepth();
}
