#include <functional>

#include "schedulers/simple_scheduler.hpp"

#include "logging/easylogging++.h"

SimpleScheduler::SimpleScheduler(std::vector<std::unique_ptr<INode>>& nodes) : BaseScheduler(nodes) {}

void SimpleScheduler::start(unsigned nrRounds) {
    for (unsigned i = 0; i < nrRounds; i++) {
        clockTick();
    }
}

const std::vector<std::unique_ptr<INode>>& SimpleScheduler::getNodes() const { return this->nodes; }

void SimpleScheduler::clockTick() {
    LOG(INFO) << "[SimpleScheduler]: " << "Broadcast time " << timeSinceStart;
    broadcastTime();

    LOG(INFO) << "[SimpleScheduler]: " << "Sending pending " << messages.size() << " messages";
    messages.tryEmptying(std::bind(&SimpleScheduler::sendRec, this, std::placeholders::_1));

    timeSinceStart++;
}

void SimpleScheduler::broadcastTime() {
    for (auto& node: nodes)
        messages.push(node->atTime(timeSinceStart));
}
