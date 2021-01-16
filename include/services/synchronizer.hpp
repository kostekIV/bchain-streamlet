#pragma once

#include "schedulers/partitioning_scheduler.hpp"

class Synchronizer: public ISynchronizationDecider {
public:
    Synchronizer(unsigned synchronizeEveryN);

    bool shouldSynchronize(unsigned round) override;
private:
    unsigned synchronizeEveryN;
};
