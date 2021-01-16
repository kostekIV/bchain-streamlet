#pragma once

#include <iostream>

enum class ServiceType {
    NORMAL
};

inline std::ostream& operator<<(std::ostream& os, const ServiceType& serviceType) {
    switch (serviceType) {
        case ServiceType::NORMAL:
            os << "RepeatService";
            break;
        default:
            throw "Unknow ServiceType";
    }
    return os;
}

