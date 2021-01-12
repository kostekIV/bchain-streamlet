#pragma once

#include <functional>
#include <vector>
#include <string>
#include <random>

#include "state/hashable.hpp"
#include "state/block.hpp"

class RoundService {
public:
    RoundService(unsigned n);

    unsigned getLeader(unsigned epoch) const;

    unsigned getEpoch(unsigned round) const;

    bool isNewEpoch(unsigned round) const;

    static bool finalizationPredicate(const std::vector<std::reference_wrapper<const Hashable>>& blocks);

    static std::string getRandomPayload();

    const Block& allocateBlock(Block && block);
private:
    const unsigned n;
    std::vector<Block> block_pool;    

    static std::mt19937 generator;
    static std::string chars;
    static unsigned EPOCH_LENGTH;
    static unsigned PAYLOAD_LENGTH;
};
