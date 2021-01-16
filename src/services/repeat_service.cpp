#include "services/repeat_service.hpp"



RepeatService::RepeatService(unsigned n,
            unsigned epochLength,
            unsigned repeatLeaderTimes,
            std::random_device::result_type r):
    AbstractService(n, epochLength, r),
    repeatLeaderTimes(repeatLeaderTimes) {}

unsigned RepeatService::getLeader(unsigned epoch) const {
    if (epoch == 0)
        throw "Genesis epoch does not have a leader";
    return ((epoch - 1) / repeatLeaderTimes) % n;
}
