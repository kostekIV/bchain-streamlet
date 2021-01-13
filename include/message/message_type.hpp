#pragma once
#include <string>

enum class MessageType{
    PROPOSAL,
    VOTE
};

inline std::string typeToString(const MessageType& type) {
    switch (type) {
        case MessageType::PROPOSAL:
            return "PROPOSAL";
            break;
        case MessageType::VOTE:
            return "VOTE";
            break;
        default:
            return "UNKNOWN MESSAGE TYPE";
            break;
    }
}
