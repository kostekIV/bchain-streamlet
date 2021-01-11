#include "message/content.hpp"

Content::Content(MessageType messageType, Block block):
    messageType(messageType),
    block(block) {}
