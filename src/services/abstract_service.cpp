#include "services/abstract_service.hpp"

const std::string AbstractService::CHARS("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
const unsigned AbstractService::MAX_PAYLOAD_LENGTH(128);

AbstractService::AbstractService(unsigned n, unsigned epochLenght, std::random_device::result_type r):
    n(n),
    epochLength(epochLenght),
    generator(r) {}

unsigned AbstractService::getEpoch(unsigned round) const {
    return round / epochLength + 1;
}

bool AbstractService::isEpochStart(unsigned round) const {
    return round % epochLength == 0;
}

std::string AbstractService::getRandomPayload() const {
    std::uniform_int_distribution<int> distribution(0, MAX_PAYLOAD_LENGTH - 1);
    unsigned length = distribution(generator) + 1;
    std::string payload(length, '0');
    for(int i = 0; i < length; i++)
        payload[i] = CHARS[distribution(generator)];
    return payload;
}

AbstractService::~AbstractService() {}
