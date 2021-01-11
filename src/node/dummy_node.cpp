#include "node/dummy_node.hpp"


std::vector<Message> DummyNode::onMessageReceive(const Message& message) {
    // do nothing
    return std::vector<Message>{};
}

std::vector<Message> DummyNode::atTime(unsigned t) {
    // do nothing
    return std::vector<Message>{};
}
