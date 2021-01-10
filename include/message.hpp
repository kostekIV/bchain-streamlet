#pragma once

#include "content.hpp"

class Message {
public:
    Message(unsigned from, unsigned to, Content content);
    unsigned from() const;
    unsigned to() const;
    Content content() const;
private:
    unsigned fromNode;
    unsigned toNode;
    Content messageContent;
};
