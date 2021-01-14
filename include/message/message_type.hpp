#pragma once
#include <string>

enum class MessageType {
    PROPOSAL,
    VOTE
};

inline std::string typeToString(MessageType type) {
    switch (type) {
        case MessageType::PROPOSAL:
            return "PROPOSAL";
            break;
        case MessageType::VOTE:
            return "VOTE";
            break;
        default:
            throw "Unknown message type";
            break;
    }
}
