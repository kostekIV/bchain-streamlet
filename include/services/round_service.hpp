#pragma once

#include "abstract_service.hpp"

class RoundService: public AbstractService {
public:
    using AbstractService::AbstractService;

    unsigned getLeader(unsigned epoch) const override;
};