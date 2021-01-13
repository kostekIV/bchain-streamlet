#pragma once

#include <functional>
#include "state/hashable.hpp"


struct Block : Hashable {
    Block(hash_t parentHash, unsigned epoch, std::string payload);

    const hash_t parentHash;
    const unsigned epoch;
    const std::string payload;

    hash_t hash() const override;

    static const Block& castFromHashable(const std::reference_wrapper<const Hashable>& ref);
};
