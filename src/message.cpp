#include "message.hpp"

Message::Message(unsigned int from, unsigned int to, Content content):
    fromNode(from),
    toNode(to),
    c(content) {}

unsigned int Message::from() const {
    return fromNode;
}

unsigned int Message::to() const {
    return toNode;
}

Content Message::content() const {
  return c;
}
