#include "schedulers/partitioning_scheduler.hpp"

#include "logging/easylogging++.h"


namespace {
    template<typename T>
    bool check_range(T start, T end, T val) {
        return start <= val and val <= end;
    }
}

PartitioningScheduler::PartitioningScheduler(std::vector<std::unique_ptr<INode>>& nodes) :
        nodes(std::move(nodes)),
        timeSinceStart(0) {}

std::vector<std::unique_ptr<INode>> PartitioningScheduler::takeOverNodes() { return std::move(this->nodes); }

void PartitioningScheduler::start(unsigned nrRounds) {
    // build the first block together
    commonClockTick();

}

void PartitioningScheduler::commonClockTick() {
    for (auto& n: nodes)
        push_to_queue(messagesCommon, n->atTime(timeSinceStart));


}

void PartitioningScheduler::clockTick() {
    broadcastTime();


    timeSinceStart++;
}


void PartitioningScheduler::broadcastTime() {

}

std::vector<Message> PartitioningScheduler::sendRec(const Message& message) {
    unsigned n = nodes.size();
    if (not check_range(0u, n - 1, message.from()) or not check_range(0u, n - 1, message.to())) {
        return std::vector<Message>{};
    }

    return nodes[message.to()]->onMessageReceive(message);
}
