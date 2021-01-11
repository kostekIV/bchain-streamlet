#pragma once

#include <vector>
#include <functional>
#include <unordered_map>
#include "vertex.hpp"

class Tree {
    using pred_t = std::function<bool(std::vector<std::reference_wrapper<const Hashable>>)>;
public:
    Tree(const Hashable& rootContent, pred_t finalizePredicate);

    const Hashable& getDeepestNotarized() const;

    void addBelow(const Hashable& parent, const Hashable& child);

    void notarize(const Hashable& hashable);

    std::vector<std::reference_wrapper<const Hashable>> getFinalizedChain() const;

    void render() const;

private:
    void tryFinalizeUntil(const Vertex& v);
    static std::vector<std::reference_wrapper<const Vertex>> getPathFromRootTo(const Vertex& v);

    const pred_t finalizationPredicate;
    const hash_t rootHash;
    std::unordered_map<hash_t, const Vertex> hvMapping;
};
