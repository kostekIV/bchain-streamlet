#include <algorithm>
#include "node/dag/dag.hpp"

#define range(V) (V).begin(), (V).end()

Dag::Dag(const Hashable& rootContent, std::function<bool(std::vector<const Hashable*>)> finalizePredicate) :
        rootHash(std::move(rootContent.hash())),
        finalizePredicate(std::move(finalizePredicate)) {
    hvMapping.emplace(rootHash, Vertex(rootContent));
}

const Hashable& Dag::getDeepestLeaf() const {
    auto argmax = std::max_element(range(hvMapping), [](auto e1, auto e2) {
        return e1.second.getDepth() < e2.second.getDepth();
    });
    return argmax->second.getContent();
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
    std::transform(range(path), hashables.begin(), [](auto v) { return &v->getContent(); });
    if (finalizePredicate(hashables))
        for (int i = 0; i < path.size() - 1; ++i)
            path[i]->finalize();
}

std::vector<const Vertex*> Dag::getPathFromRootTo(const Vertex& v) {
    auto current = &v, next = &current->getParent();
    std::vector<const Vertex*> path = {current};
    do {
        current = next;
        next = &current->getParent();
        path.push_back(current);
    } while (current != next);
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
    std::transform(range(finalized), result.begin(), [](auto v) {
        return &v->getContent();
    });
    return result;
}
