#include <algorithm>
#include <utility>
#include "node/tree/tree.hpp"

#define range(V) (V).begin(), (V).end()

Tree::Tree(const Hashable& rootContent, pred_t finalizationPredicate) :
        finalizationPredicate(std::move(finalizationPredicate)),
        rootHash(std::move(rootContent.hash())),
        hvMapping{{rootHash, rootContent}},
        deepestNotarized(hvMapping.at(rootHash)),
        deepestFinalized(deepestNotarized) {
    auto& root = hvMapping.at(rootHash);
    root.notarize();
    root.finalize();
}

bool Tree::isRoot(const Vertex& v) { return v.getDepth() == 0; }

const Hashable& Tree::getDeepestNotarized() const {
    return deepestNotarized.get().getContent();
}

void Tree::addBelow(const Hashable& parent, const Hashable& child) {
    const Vertex& parentVertex = hvMapping.at(parent.hash());
    hash_t childHash = child.hash();
    hvMapping.try_emplace(childHash, child, parentVertex);
}

void Tree::notarize(const Hashable& hashable) {
    const Vertex& v = hvMapping.at(hashable.hash());
    v.notarize();
    if (v.getDepth() > deepestNotarized.get().getDepth())
        deepestNotarized = v;
    tryFinalizeUntil(v);
}

void Tree::tryFinalizeUntil(const Vertex& v) {
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

void Tree::render() const {}

Tree::hashables_t Tree::getFinalizedChain() const {
    auto finalized = getPathFromRootTo(deepestFinalized);
    hashables_t result;
    std::for_each(range(finalized), [&result](auto v) { result.push_back(v.get().getContent()); });
    return result;
}
