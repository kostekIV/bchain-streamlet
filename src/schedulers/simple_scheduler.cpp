#include <functional>

#include "schedulers/simple_scheduler.hpp"

#include "logging/easylogging++.h"

namespace {
    template <typename T>
    void insert(std::vector<T>& dest, std::vector<T>&& src) {
        for (auto& el: src) {
            dest.push_back(el);
        }
    }
}

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
    sendFromRound(timeSinceStart);
    timeSinceStart++;
    sendFromRound(timeSinceStart);
}

void SimpleScheduler::broadcastTime() {
    auto& thisRoundMessages = roundMessages[timeSinceStart];
    for (auto& node: nodes) {
        insert(thisRoundMessages, node->atTime(timeSinceStart));
    }
}

void SimpleScheduler::sendFromRound(unsigned round) {
    auto& messages = roundMessages[round];
    auto& nextRoundMessages = roundMessages[round + 1];

    LOG(INFO) << "[SimpleScheduler]: " << "Sending pending " << messages.size() << " messages";
    for (auto& message: messages) {
        insert(nextRoundMessages, sendRec(message));
    }

    roundMessages.erase(round);
};
