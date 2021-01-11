#pragma once

#include "message_type.hpp"
#include "node/block.hpp"

struct Content {
    const MessageType messageType;
    const Block block;
    Content(MessageType messageType, Block block);
};
