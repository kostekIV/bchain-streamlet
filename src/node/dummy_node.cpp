
#include "logging/easylogging++.h"
#include "node/dummy_node.hpp"


std::vector<Message> DummyNode::onMessageReceive(const Message& message) {
    // do nothing
    LOG(DEBUG) << "Dummy Node: received message from " << message.from();
    return std::vector<Message>{};
}

std::vector<Message> DummyNode::atTime(unsigned t) {
    // do nothing
    LOG(DEBUG) << "Dummy Node: received atTime: " << t;
    return std::vector<Message>{};
}
