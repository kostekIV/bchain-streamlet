#include "round_service.hpp"

unsigned RoundService::PAYLOAD_LENGTH(16);
unsigned RoundService::EPOCH_LENGTH(2);
std::mt19937 RoundService::generator(std::random_device{}());
std::string RoundService::chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

RoundService::RoundService(unsigned n) : n(n) {}

unsigned RoundService::getLeader(unsigned epoch) const {
    return epoch % n;
}

bool RoundService::finalizationPredicate(const std::vector<std::reference_wrapper<const Hashable>>& blocks) {
    size_t len = blocks.size();
    if (len < 3) return false;
    int a = Block::castFromHashable(blocks[len - 3]).epoch;
    int b = Block::castFromHashable(blocks[len - 2]).epoch;
    int c = Block::castFromHashable(blocks[len - 1]).epoch;
    return a + 1 == b and b + 1 == c;
}

unsigned RoundService::getEpoch(unsigned round) const {
    return round/EPOCH_LENGTH + 1;
}

bool RoundService::isNewEpoch(unsigned round) const {
    return round % EPOCH_LENGTH == 0;
}

std::string RoundService::getRandomPayload() {
    std::uniform_int_distribution<int> distribution(0, PAYLOAD_LENGTH-1);
    std::string payload(PAYLOAD_LENGTH, '0');
    for(int i = 0; i < PAYLOAD_LENGTH; i++)
        payload[i] = distribution(generator);
    return payload;
}