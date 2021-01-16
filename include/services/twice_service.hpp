#pragma once

#include "round_service.hpp"

class TwiceService: public AbstractService {
public:
    using AbstractService::AbstractService;

    unsigned getLeader(unsigned epoch) const override;
};