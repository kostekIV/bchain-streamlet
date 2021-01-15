#pragma once

#include <iostream>

enum class ServiceType {
    NORMAL,
    TWICE
};

inline std::ostream& operator<<(std::ostream& os, const ServiceType& serviceType) {
    switch (serviceType) {
        case ServiceType::NORMAL:
            os << "RoundService";
            break;
        case ServiceType::TWICE:
            os << "TwiceSerivce"; 
        default:
            throw "Unknow ServiceType";
    }
    return os;
}

