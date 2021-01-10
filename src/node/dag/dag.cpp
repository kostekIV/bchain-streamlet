#include "node/dag/dag.hpp"


Dag::Dag(const Hashable& root_content) : root(root_content) {

}

const Hashable& Dag::getDeepestLeaf() const {

}

void Dag::addBelow(const Hashable& parent, const Hashable& child) {

}

void Dag::notarize(const Hashable& hashable) {

}

void Dag::render() const {

}

std::vector<Hashable> Dag::getFinalizedChain() const {
    return std::vector<Hashable>();
}
