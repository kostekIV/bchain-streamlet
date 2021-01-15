#pragma once

#include <functional>
#include "state/hashable.hpp"


class Block : public Hashable {
public:
    static Block createGenesisBlock();

    static const Block& castFromHashable(const std::reference_wrapper<const Hashable>& ref);

    Block(hash_t parentHash, unsigned epoch, std::string payload);

    hash_t getParentHash() const;

    unsigned getEpoch() const;

    std::string getPayload() const;

    hash_t hash() const override;

protected:
    const hash_t parentHash;
    const unsigned epoch;
    const std::string payload;
};
