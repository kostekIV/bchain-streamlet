#include "schedulers/partitioning_scheduler.hpp"

#include "logging/easylogging++.h"

PartitioningScheduler::PartitioningScheduler(std::vector<std::unique_ptr<INode>>& nodes,
                                             std::unique_ptr<ISynchronizationDecider>& decider):
        BaseScheduler(nodes),
        decider(std::move(decider)),
        action(*this) {
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
            minorityIds.insert(i);
        }
    }

    LOG(DEBUG) << "PartitioningScheduler initialized with " << nrOfNodes << " from which " << minority << " are in minority";
}

void PartitioningScheduler::start(unsigned nrRounds) {
    for (unsigned i = 0; i < nrRounds; i++) {
        clockTick();
    }
}

void PartitioningScheduler::redirectMessagesToQueue(std::vector<Message>&& messages, unsigned round) {
    for (const auto& m: messages) {

        switch (getEdgeType(m)) {
            case MAJ_TO_MAJ:
                messagesMaj2Maj.push({round, m});
                break;
            case MAJ_TO_MIN:
                messagesMaj2Min.push({round, m});
                break;
            case MIN_TO_MIN:
                messagesMin2Min.push({round, m});
                break;
            case MIN_TO_MAJ:
                messagesMin2Maj.push({round, m});
                break;
        }
    }
}

void PartitioningScheduler::clockTick() {
    LOG(INFO) << "Broadcast time " << timeSinceStart;
    broadcastTime();

    beforeAndAfter();
    timeSinceStart++;
    beforeAndAfter();
}

void PartitioningScheduler::beforeAndAfter() {
    messagesMaj2Maj.applyToAll(action);
    messagesMin2Min.applyToAll(action);

    if (decider->shouldSynchronize(timeSinceStart)) {
        LOG(INFO) << "Synchronising nodes in round: " << timeSinceStart;
        messagesMin2Maj.applyToAll(action);
        messagesMaj2Min.applyToAll(action);
    }

    for (const auto& mr: messages) {
        redirectMessagesToQueue({mr.second}, mr.first);
    }
    messages.clear();
}

void PartitioningScheduler::broadcastTime() {
    for (auto& node: nodes) {
        redirectMessagesToQueue(node->atTime(timeSinceStart), timeSinceStart);
    }
}

PartitioningScheduler::EdgeType PartitioningScheduler::getEdgeType(const Message& m) {
    auto from = m.from();
    auto to = m.to();

    if (minorityIds.find(from) != minorityIds.end()) {
        if (minorityIds.find(to) != minorityIds.end()) {
            return MIN_TO_MIN;
        } else {
            return MIN_TO_MAJ;
        }
    } else {
        if (minorityIds.find(to) != minorityIds.end()) {
            return MAJ_TO_MIN;
        } else {
            return MAJ_TO_MAJ;
        }
    }
}

void PartitioningScheduler::Action::onPop(std::pair<unsigned, Message> roundMessage) {
    std::vector<std::pair<unsigned, Message>> responses;
    auto res = scheduler.sendRec(roundMessage.second);
    auto round = roundMessage.first;

    if (round <= scheduler.timeSinceStart) {
        scheduler.redirectMessagesToQueue(std::move(res), round + 1);
    } else {
        scheduler.messages.emplace_back(round + 1, roundMessage.second);
    }
}

PartitioningScheduler::Action::Action(PartitioningScheduler& scheduler):
    scheduler(scheduler) {}

ISynchronizationDecider::~ISynchronizationDecider() {}
