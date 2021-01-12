#include "message/content.hpp"

Content::Content(MessageType messageType, const Block& block):
    messageType(messageType),
    block(block) {}
