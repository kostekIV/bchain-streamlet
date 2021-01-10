#pragma once

#include <vector>

#include "node.hpp"

class DummyNode: public INode {
public:
    std::vector<Message> onMessageReceive(Message &message);
    std::vector<Message> atTime(unsigned int t);
};
