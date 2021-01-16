#pragma once

#include <functional>
#include "status.hpp"

using hash_t = std::string;

class Block {
public:
    static Block createGenesisBlock();

    static hash_t EMPTY_HASH;

    Block(hash_t parentHash, unsigned epoch, std::string payload);

    virtual hash_t getParentHash() const;

    virtual unsigned getEpoch() const;

    virtual std::string getPayload() const;

    virtual Status getStatus() const;

    virtual void notarize() const;

    virtual void finalize() const;

    virtual hash_t hash() const;

protected:
    const hash_t parentHash;
    const unsigned epoch;
    const std::string payload;
    mutable Status status = Status::PRESENT;
};
