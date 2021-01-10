#pragma once

#include <vector>
#include "vertex.hpp"

class Dag {
public:
    explicit Dag(const Hashable& root_content);

    const Hashable& getDeepestLeaf() const;
    void addBelow(const Hashable& parent, const Hashable& child);
    void notarize(const Hashable& hashable);

    void render() const;
    std::vector<Hashable> getFinalizedChain() const;

private:
    const Vertex& root;
};
