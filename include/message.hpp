#pragma once

#include "content.hpp"

class Message {
private:
    const unsigned int fromNode;
    const unsigned int toNode;
    const Content c;
public:
    Message(unsigned int from, unsigned int to, Content content);
    unsigned int from() const;
    unsigned int to() const;
    Content content() const;
};
