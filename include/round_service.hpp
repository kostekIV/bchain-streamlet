#pragma once

#include <functional>
#include <vector>
#include <string>
#include <random>

#include "state/hashable.hpp"
#include "state/block.hpp"

class RoundService {
public:
    explicit RoundService(unsigned n, 
                unsigned epochLength = 2, 
                std::random_device::result_type r = std::random_device{}());

    unsigned getLeader(unsigned epoch) const;

    unsigned getEpoch(unsigned round) const;

    bool isEpochStart(unsigned round) const;

    std::string getRandomPayload();

private:
    const unsigned n;
    unsigned epochLength;
    std::default_random_engine generator;
    
    static const std::string CHARS;
};
