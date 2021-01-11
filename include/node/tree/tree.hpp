#pragma once

#include <vector>
#include <functional>
#include <unordered_map>
#include "vertex.hpp"

class Tree {
private:
    using vertices_t = std::vector<std::reference_wrapper<const Vertex>>;
    using hashables_t = std::vector<std::reference_wrapper<const Hashable>>;
    using pred_t = std::function<bool(hashables_t)>;
public:
    Tree(const Hashable& rootContent, pred_t finalizePredicate);

    const Hashable& getDeepestNotarized() const;

    void addBelow(const Hashable& parent, const Hashable& child);

    void notarize(const Hashable& hashable);

    hashables_t getFinalizedChain() const;

    void render() const;

private:
    void tryFinalizeUntil(const Vertex& v);

    static vertices_t getPathFromRootTo(const Vertex& v);

    static bool isRoot(const Vertex& v);

    const pred_t finalizationPredicate;
    const hash_t rootHash;
    std::unordered_map<hash_t, const Vertex> hvMapping;
    std::reference_wrapper<const Vertex> deepestNotarized, deepestFinalized;
};
