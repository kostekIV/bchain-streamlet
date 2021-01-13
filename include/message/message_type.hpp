#pragma once
#include <string>

enum class MessageType{
    PROPOSAL,
    VOTE
};

inline std::string typeToString(const MessageType& status) {
    switch (status) {
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
