#pragma once

#include <vector>
#include <functional>
#include <unordered_map>
#include "vertex.hpp"

class Tree {
public:
    Tree(const Hashable& rootContent, std::function<bool(std::vector<const Hashable*>)> finalizePredicate);

    const Hashable& getDeepestNotarized() const;

    void addBelow(const Hashable& parent, const Hashable& child);

    void notarize(const Hashable& hashable);

    std::vector<const Hashable*> getFinalizedChain() const;

    void render() const;

private:
    void tryFinalizeUntil(const Vertex& v);
    static std::vector<const Vertex*> getPathFromRootTo(const Vertex& v);

    const std::function<bool(std::vector<const Hashable*>)> finalizationPredicate;
    const hash_t rootHash;
    std::unordered_map<hash_t, const Vertex> hvMapping;
};
