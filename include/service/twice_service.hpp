#pragma once

#include "round_service.hpp"

class TwiceService: public RoundService {
public:
    using RoundService::RoundService;

    unsigned getLeader(unsigned epoch) const override;
};