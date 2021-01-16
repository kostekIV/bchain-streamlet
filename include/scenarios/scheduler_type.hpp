#pragma once

#include <iostream>

enum class SchedulerType {
    SIMPLE,
    PARTITIONING
};

inline std::ostream& operator<<(std::ostream& os, const SchedulerType& schedulerType) {
    if (schedulerType == SchedulerType::SIMPLE) {
        os << "SimpleScheduler";
    } else if (schedulerType == SchedulerType::PARTITIONING) {
        os << "PartitioningScheduler";
    }
    return os;
}

