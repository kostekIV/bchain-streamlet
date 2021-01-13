#include "round_service.hpp"

const std::string RoundService::CHARS("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");

RoundService::RoundService(unsigned n, unsigned epochLenght, std::random_device::result_type r):
    n(n),
    epochLength(epochLenght),
    generator(r){}

unsigned RoundService::getLeader(unsigned epoch) const {
    return (epoch - 1) % n;
}

unsigned RoundService::getEpoch(unsigned round) const {
    return round / epochLength + 1;
}

bool RoundService::isEpochStart(unsigned round) const {
    return round % epochLength == 0;
}

std::string RoundService::getRandomPayload() {
    std::uniform_int_distribution<int> distribution(0, CHARS.length()-1);
    unsigned length = distribution(generator) + 1;
    std::string payload(length, '0');
    for(int i = 0; i < length; i++)
        payload[i] = CHARS[distribution(generator)];
    return payload;
}
