#include "node/node.hpp"

INode::~INode() {}

bool INode::finalizationPredicate(const std::vector<std::reference_wrapper<const Block>>& blocks) {
    size_t len = blocks.size();
    if (len < 3) return false;
    unsigned a = blocks[len - 3].get().getEpoch();
    unsigned b = blocks[len - 2].get().getEpoch();
    unsigned c = blocks[len - 1].get().getEpoch();
    return a + 1 == b and b + 1 == c;
}