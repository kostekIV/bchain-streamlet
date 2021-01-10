#include "dummy_node.hpp"


std::vector<Message> DummyNode::onMessageReceive(Message &message) {
    // do nothing
    return std::vector<Message>{};
}

std::vector<Message> DummyNode::atTime(unsigned int t) {
    // do nothing
    return std::vector<Message>{};
}
