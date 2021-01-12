#pragma once

#include <iostream>

enum class SchedulerType {
    SIMPLE
};

inline std::ostream& operator<<(std::ostream& os, const SchedulerType& schedulerType) {
    if (schedulerType == SchedulerType::SIMPLE) {
        os << "SimpleScheduler";
    }
    return os;
}

