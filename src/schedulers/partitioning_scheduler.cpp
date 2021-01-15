#include "schedulers/partitioning_scheduler.hpp"

#include "logging/easylogging++.h"

PartitioningScheduler::PartitioningScheduler(std::vector<std::unique_ptr<INode>>& nodes) : IScheduler(nodes) {}

void PartitioningScheduler::start(unsigned nrRounds) {
    // build the first block together
    commonClockTick();
    // ...
}

void PartitioningScheduler::commonClockTick() {
    for (auto& node: nodes)
        messagesCommon.push(node->atTime(timeSinceStart));
}

void PartitioningScheduler::clockTick() {
    // TODO
}


void PartitioningScheduler::broadcastTime() {
    // TODO
}
