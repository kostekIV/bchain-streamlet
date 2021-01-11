#pragma once

#include "hashable.hpp"


struct Block : Hashable {
    Block(hash_t parentHash, unsigned epoch, std::string payload);

    const hash_t parentHash;
    const unsigned epoch;
    const std::string payload;

    hash_t hash() const override;
};
