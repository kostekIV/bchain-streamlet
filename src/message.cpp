#include "message.hpp"

Message::Message(unsigned from, unsigned to, Content content):
    fromNode(from),
    toNode(to),
    messageContent(content) {}

unsigned Message::from() const {
    return fromNode;
}

unsigned Message::to() const {
    return toNode;
}

Content Message::content() const {
    return messageContent;
}
