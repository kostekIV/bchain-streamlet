#include "schedulers/partitioning_scheduler.hpp"

#include "logging/easylogging++.h"

PartitioningScheduler::PartitioningScheduler(std::vector<std::unique_ptr<INode>>& nodes): BaseScheduler(nodes) {
    initialize();
}

void PartitioningScheduler::initialize() {
    auto nrOfNodes = nodes.size();

    if (nrOfNodes < 4) {
        throw "Not enough nodes in the simulation, at least 4 is needed";
    }

    // majority = nrOfNodes - minority;
    auto minority = (nrOfNodes - 1) / 3;
    for (unsigned i = 0; i < nrOfNodes; i++) {
        if (i < minority) {
            minorityIds.emplace(i);
        } else {
            majorityIds.emplace(i);
        }
    }

    LOG(DEBUG) << "PartitioningScheduler initialized with " << nrOfNodes << " from which " << minority << "are in minority";
}

void PartitioningScheduler::start(unsigned nrRounds) {
    commonClockTick();
}

void PartitioningScheduler::commonClockTick() {
    for (auto& node: nodes)
        messagesCommon.push(node->atTime(timeSinceStart));
}

void PartitioningScheduler::clockTick() {
}

void PartitioningScheduler::broadcastTime() {
}

void PartitioningScheduler::onPop(Message m) {
}
