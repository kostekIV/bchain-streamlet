#pragma once

#include <vector>
#include <functional>
#include <unordered_map>
#include "vertex.hpp"

class Dag {
public:
    Dag(const Hashable& root_content, std::function<bool(std::vector<const Hashable*>)> finalize_predicate);

    const Hashable& getDeepestLeaf() const;
    void addBelow(const Hashable& parent, const Hashable& child);
    void notarize(const Hashable& hashable);

    void render() const;
    std::vector<const Hashable*> getFinalizedChain() const;

private:
    void tryFinalizeFrom(const Vertex& v);
    std::vector<const Vertex*> getPathFrom(const Vertex& v) const;

    const std::function<bool(std::vector<const Hashable*>)> finalize_predicate;
    const hash_t root_hash;
    std::unordered_map<hash_t, const Vertex> hv_mapping;
};
