#pragma once

#include <vector>

#include "message.hpp"

class INode {
public:
    virtual std::vector<Message> onMessageReceive(Message &message) = 0;
    virtual std::vector<Message> atTime(unsigned int t) = 0;
};
