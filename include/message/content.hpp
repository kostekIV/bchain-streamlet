#pragma once

#include "message/message_type.hpp"
#include "state/block.hpp"

struct Content {
    const MessageType messageType;
    const Block& block;
    Content(MessageType messageType, const Block& block);
};
