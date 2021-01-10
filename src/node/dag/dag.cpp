#include <algorithm>
#include "node/dag/dag.hpp"

#define range(V) (V).begin(), (V).end()

Dag::Dag(const Hashable& root_content, std::function<bool(std::vector<const Hashable*>)> finalize_predicate) :
        root_hash(std::move(root_content.hash())),
        finalize_predicate(std::move(finalize_predicate)) {
    hv_mapping.emplace(root_hash, Vertex(root_content));
}

const Hashable& Dag::getDeepestLeaf() const {
    auto argmax = std::max_element(range(hv_mapping), [](auto e1, auto e2) {
        return e1.second.getDepth() < e2.second.getDepth();
    });
    return argmax->second.getContent();
}

void Dag::addBelow(const Hashable& parent, const Hashable& child) {
    const Vertex& parent_vertex = hv_mapping.at(parent.hash());
    hash_t child_hash = child.hash();
    hv_mapping.insert({child_hash, Vertex(child, parent_vertex)});
}

void Dag::notarize(const Hashable& hashable) {
    const Vertex& v = hv_mapping.at(hashable.hash());
    v.notarize();
    tryFinalizeFrom(v);
}

void Dag::tryFinalizeFrom(const Vertex& v) {
    auto path = getPathFrom(v);
    std::vector<const Hashable*> hashables;
    std::transform(range(path), hashables.begin(), [](auto v) { return &v->getContent(); });
    if (finalize_predicate(hashables))
        for (int i = 0; i < path.size() - 1; ++i)
            path[i]->finalize();
}

std::vector<const Vertex*> Dag::getPathFrom(const Vertex& v) const {
    std::vector<const Vertex*> path;
    auto current = &v, next = &current->getParent();
    while (current != next) {
        path.push_back(current);
        current = next;
        next = &current->getParent();
    }
    return path;
}

void Dag::render() const {}

std::vector<const Hashable*> Dag::getFinalizedChain() const {
    std::vector<const Vertex*> finalized;
    for (const auto&[key, val] : hv_mapping)
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
