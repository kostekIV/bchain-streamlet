#pragma once

#include <iostream>

enum class ServiceType {
    REPEATING_LEADER
};

inline std::ostream& operator<<(std::ostream& os, const ServiceType& serviceType) {
    switch (serviceType) {
        case ServiceType::REPEATING_LEADER:
            os << "RepeatService";
            break;
        default:
            throw "Unknow ServiceType";
    }
    return os;
}

