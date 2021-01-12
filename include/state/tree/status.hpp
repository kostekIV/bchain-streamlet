#pragma once
#include <string>

enum class Status {
    PRESENT,
    NOTARIZED,
    FINALIZED
};

inline std::string statusToString(const Status& status) {
    switch (status) {
        case Status::PRESENT:
            return "PRESENT";
            break;
        case Status::NOTARIZED:
            return "NOTARIZED";
            break;
        case Status::FINALIZED:
            return "FINALIZED";
            break;
        default:
            return "UNKNOWN STATUS";
            break;
    }
}
