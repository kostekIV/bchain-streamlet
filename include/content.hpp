#pragma once

#include "node/block.hpp"

struct Content {
    enum MessageType {proposal, vote};
    const MessageType messageType;
    const Block block;
    Content(MessageType messageType, Block block);
};
