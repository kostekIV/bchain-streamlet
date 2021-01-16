#include "services/round_service.hpp"

unsigned RoundService::getLeader(unsigned epoch) const {
    if(epoch == 0)
        throw "Genesis epoch does not have a leader";
    return (epoch - 1) % n;
}
