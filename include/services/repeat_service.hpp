#pragma once

#include "services/abstract_service.hpp"

class RepeatService: public AbstractService {
public:
    explicit RepeatService(unsigned n,
                unsigned epochLength = 2,
                unsigned repeatLeaderTimes = 1,
                std::random_device::result_type r = std::random_device{}());

    unsigned getLeader(unsigned epoch) const override;

private:
    unsigned repeatLeaderTimes;
};
