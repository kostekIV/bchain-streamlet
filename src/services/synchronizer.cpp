#include "services/synchronizer.hpp"

Synchronizer::Synchronizer(unsigned synchronizeEveryN):
    synchronizeEveryN(synchronizeEveryN) {}

bool Synchronizer::shouldSynchronize(unsigned round) {
    return round % synchronizeEveryN == 0;
}
