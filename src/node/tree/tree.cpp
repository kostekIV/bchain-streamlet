#include <algorithm>
#include <utility>
#include <iostream>
#include "node/tree/tree.hpp"

#define range(V) (V).begin(), (V).end()

Tree::Tree(const Hashable &rootContent, pred_t finalizationPredicate) :
        rootHash(std::move(rootContent.hash())),
        finalizationPredicate(std::move(finalizationPredicate)) {
    hvMapping.emplace(rootHash, Vertex(rootContent));
}

const Hashable &Tree::getDeepestNotarized() const {
    auto best = std::cref(hvMapping.at(rootHash));
    for (const auto&[key, val] : hvMapping)
        if (val.getStatus() == Status::NOTARIZED and val.getDepth() > best.get().getDepth())
            best = val;
    return best.get().getContent();
}

void Tree::addBelow(const Hashable &parent, const Hashable &child) {
    const Vertex &parentVertex = hvMapping.at(parent.hash());
    hash_t childHash = child.hash();
    hvMapping.insert({childHash, Vertex(child, parentVertex)});
}

void Tree::notarize(const Hashable &hashable) {
    const Vertex &v = hvMapping.at(hashable.hash());
    v.notarize();
    tryFinalizeUntil(v);
}

void Tree::tryFinalizeUntil(const Vertex &v) {
    auto path = getPathFromRootTo(v);
    std::vector<std::reference_wrapper<const Hashable>> hashables;
    std::for_each(range(path), [&hashables](auto v) { hashables.push_back(std::cref(v.get().getContent())); });
    if (finalizationPredicate(hashables))
        for (int i = 0; i < path.size() - 1; ++i)
            path[i].get().finalize();
}

std::vector<std::reference_wrapper<const Vertex>> Tree::getPathFromRootTo(const Vertex &v) {
    auto current = std::cref(v);
    std::vector<std::reference_wrapper<const Vertex>> path = {current};
    if (current.get().getDepth() == 0) return path;
    do {
        current = current.get().getParent();
        path.push_back(current);
    } while (current.get().getDepth() > 0);
    std::reverse(range(path));
    return path;
}

void Tree::render() const {}

std::vector<std::reference_wrapper<const Hashable>> Tree::getFinalizedChain() const {
    std::vector<std::reference_wrapper<const Vertex>> finalized;
    for (const auto&[key, val] : hvMapping)
        if (val.getStatus() == Status::FINALIZED)
            finalized.push_back(val);
    std::sort(range(finalized), [](auto u, auto v) {
        return u.get().getDepth() < v.get().getDepth();
    });

    std::vector<std::reference_wrapper<const Hashable>> result;
    std::for_each(range(finalized), [&result](auto v) { result.push_back(v.get().getContent()); });
    return result;
}
