#pragma once

#include <vector>

#include "node.hpp"

class DummyNode: public INode {
public:
    std::vector<Message> onMessageReceive(const Message& message) override;
    std::vector<Message> atTime(unsigned t) override;
    ~DummyNode() = default;
};
