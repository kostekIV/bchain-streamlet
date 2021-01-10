#pragma once

#include "content.hpp"

class Message {
public:
    Message(unsigned from, unsigned to, Content content);
    unsigned from() const;
    unsigned to() const;
    Content content() const;
private:
    const unsigned fromNode;
    const unsigned toNode;
    const Content messageContent;
};
