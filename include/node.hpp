#pragma once

#include <vector>

#include "message.hpp"

class INode {
public:
    virtual std::vector<Message> onMessageReceive(const Message& message) = 0;
    virtual std::vector<Message> atTime(unsigned t) = 0;
    virtual ~INode() = 0;
};
