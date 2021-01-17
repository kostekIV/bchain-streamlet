#pragma once

#include <vector>

#include "node.hpp"

class BaseNode : public INode {
public:
    virtual std::vector<Message> onMessageReceive(const Message& message) = 0;
    
    virtual std::vector<Message> atTime(unsigned t) = 0;
protected:
    static bool finalizationPredicate(const std::vector<std::reference_wrapper<const Block>>& blocks);
};
