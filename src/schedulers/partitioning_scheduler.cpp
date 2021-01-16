#include "schedulers/partitioning_scheduler.hpp"

#include "logging/easylogging++.h"

PartitioningScheduler::PartitioningScheduler(std::vector<std::unique_ptr<INode>>& nodes,
                                             std::unique_ptr<ISynchronizationDecider>& decider):
        BaseScheduler(nodes),
        decider(std::move(decider)) {
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
        } else {
            majorityIds.insert(i);
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
        auto from = m.from();
        auto to = m.from();

        switch (getEdgeType(m)) {
            case MAJ_TO_MAJ:
                messagesMaj2Maj.push(std::make_pair(round, m));
                break;
            case MAJ_TO_MIN:
                messagesMaj2Min.push(std::make_pair(round, m));
                break;
            case MIN_TO_MIN:
                messagesMin2Min.push(std::make_pair(round, m));
                break;
            case MIN_TO_MAJ:
                messagesMin2Maj.push(std::make_pair(round, m));
                break;
        }
    }
}

void PartitioningScheduler::clockTick() {
    LOG(INFO) << "Broadcast time " << timeSinceStart;
    broadcastTime();

    messagesMaj2Maj.applyToAll(*this);
    messagesMin2Min.applyToAll(*this);

    if (decider->shouldSynchronize(timeSinceStart)) {
        LOG(INFO) << "Synchronising nodes in round: " << timeSinceStart;
        messagesMin2Maj.applyToAll(*this);
        messagesMaj2Min.applyToAll(*this);
    }

    for (const auto& mr: messages) {
        redirectMessagesToQueue({mr.second}, mr.first);
    }
    messages.clear();
    timeSinceStart++;
}

void PartitioningScheduler::broadcastTime() {
    for (auto& node: nodes) {
        redirectMessagesToQueue(node->atTime(timeSinceStart), timeSinceStart + 1);
    }
}

void PartitioningScheduler::onPop(std::pair<unsigned, Message> round_message) {
    std::vector<std::pair<unsigned, Message>> responses;
    auto res = sendRec(round_message.second);
    auto round = round_message.first;

    if (round < timeSinceStart) {
        redirectMessagesToQueue(std::move(res), round);
    } else if (round == timeSinceStart) {
        std::transform(res.begin(), res.end(), std::back_inserter(messages),
            [&](Message m) -> std::pair<unsigned, Message> {
                return std::make_pair(round + 1, m);
            });
    } else {
        messages.emplace_back(round + 1, round_message.second);
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

ISynchronizationDecider::~ISynchronizationDecider() {}
