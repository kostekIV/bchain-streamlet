#include "round_service.hpp"

RoundService::RoundService(unsigned n) : n(n) {}

unsigned RoundService::getLeader(unsigned epoch) const{
    return epoch % n;
}
