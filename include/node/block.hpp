#pragma once

#include "node/dag/dag.hpp"
#include "node/dag/status.hpp"
#include "hashable.hpp"


struct Block : Hashable {
    Block(hash_t parent_hash, unsigned epoch, std::string payload);

    const hash_t parent_hash;
    const unsigned epoch;
    const std::string payload;

    hash_t hash() const override;
};