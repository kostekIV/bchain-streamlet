#pragma once

#include <string>
#include <random>

class RoundService {
public:
    explicit RoundService(unsigned n,
                unsigned epochLength = 2,
                std::random_device::result_type r = std::random_device{}());

    virtual unsigned getLeader(unsigned epoch) const;

    unsigned getEpoch(unsigned round) const;

    bool isEpochStart(unsigned round) const;

    std::string getRandomPayload() const;

protected:
    const unsigned n;
    const unsigned epochLength;
    mutable std::default_random_engine generator;
    
    static const std::string CHARS;
    static const unsigned MAX_PAYLOAD_LENGTH;
};
