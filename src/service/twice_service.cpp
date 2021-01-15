#include "service/twice_service.hpp"

unsigned TwiceService::getLeader(unsigned epoch) const {
    if(epoch == 0)
        throw "Genesis epoch does not have a leader";
    return ((epoch - 1) / 2) % n;
}