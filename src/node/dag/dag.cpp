#include <algorithm>
#include <utility>
#include <iostream>
#include "node/dag/dag.hpp"

#define range(V) (V).begin(), (V).end()

Dag::Dag(const Hashable& rootContent, std::function<bool(std::vector<const Hashable*>)> finalizationPredicate) :
        rootHash(std::move(rootContent.hash())),
        finalizationPredicate(std::move(finalizationPredicate)) {
    hvMapping.emplace(rootHash, Vertex(rootContent));
}

const Hashable& Dag::getDeepestNotarized() const {
    const Vertex* best = &hvMapping.at(rootHash);
    for (const auto&[key, val] : hvMapping)
        if (val.getStatus() == Status::NOTARIZED and val.getDepth() > best->getDepth())
            best = &val;
    return best->getContent();
}

void Dag::addBelow(const Hashable& parent, const Hashable& child) {
    const Vertex& parentVertex = hvMapping.at(parent.hash());
    hash_t childHash = child.hash();
    hvMapping.insert({childHash, Vertex(child, parentVertex)});
}

void Dag::notarize(const Hashable& hashable) {
    const Vertex& v = hvMapping.at(hashable.hash());
    v.notarize();
    tryFinalizeUntil(v);
}

void Dag::tryFinalizeUntil(const Vertex& v) {
    auto path = getPathFromRootTo(v);
    std::vector<const Hashable*> hashables;
    std::for_each(range(path), [&hashables](auto v) { hashables.push_back(&v->getContent()); });
    if (finalizationPredicate(hashables))
        for (int i = 0; i < path.size() - 1; ++i)
            path[i]->finalize();
}

std::vector<const Vertex*> Dag::getPathFromRootTo(const Vertex& v) {
    auto current = &v;
    std::vector<const Vertex*> path = {current};
    if (current->getDepth() == 0) return path;
    do {
        current = &current->getParent();
        path.push_back(current);
    } while (current->getDepth() > 0);
    std::reverse(range(path));
    return path;
}

void Dag::render() const {}

std::vector<const Hashable*> Dag::getFinalizedChain() const {
    std::vector<const Vertex*> finalized;
    for (const auto&[key, val] : hvMapping)
        if (val.getStatus() == Status::FINALIZED)
            finalized.push_back(&val);
    std::sort(range(finalized), [](auto u, auto v) {
        return u->getDepth() < v->getDepth();
    });

    std::vector<const Hashable*> result;
    std::for_each(range(finalized), [&result](auto v) { result.push_back(&v->getContent()); });
    return result;
}
