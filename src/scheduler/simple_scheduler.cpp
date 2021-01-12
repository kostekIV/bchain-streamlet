#include "scheduler/simple_scheduler.hpp"

#include "logging/easylogging++.h"


namespace {
    template <typename T>
    bool check_range(T start, T end, T val) {
        return start <= val and val <= end;
    }

    template <typename T>
    void push_to_queue(std::queue<T>& q, std::vector<T>&& v) {
        for (auto& x: v) {
            q.push(x);
        }
    }
}

SimpleScheduler::SimpleScheduler(std::vector<std::unique_ptr<INode>>& nodes):
    nodes(std::move(nodes)),
    timeSinceStart(0) {}

void SimpleScheduler::start(unsigned nrRounds) {
    for (unsigned i = 0; i < nrRounds; i++) {
        clockTick();
    }
}

const std::vector<std::unique_ptr<INode>>& SimpleScheduler::getNodes() const {
    return this->nodes;
}


std::vector<std::unique_ptr<INode>> SimpleScheduler::takeOverNodes() {
    return std::move(this->nodes);
}

void SimpleScheduler::clockTick() {

    LOG(INFO) << "[SimpleScheduler]: " << "Broadcast time " << timeSinceStart;
    broadcastTime();

    LOG(INFO) << "[SimpleScheduler]: " << "Sending pending " << messages.size() << " messages";
    while (!messages.empty()) {
        auto m = messages.front();
        messages.pop();
        push_to_queue(messages, sendRec(m));
    }

    timeSinceStart++;
}

void SimpleScheduler::broadcastTime() {
    for (auto& n: nodes) {
        push_to_queue(messages, n->atTime(timeSinceStart));
    }
}


std::vector<Message> SimpleScheduler::sendRec(const Message& message) {
    unsigned n = nodes.size();
    if (not check_range(0u, n - 1, message.from()) or not check_range(0u, n - 1, message.to())) {
        // ToDo add logging
        LOG(ERROR) << "Message receiver or sender not it range\n" << "Receiver: " << message.to() << ", Sender: " << message.from() << ", Node count: " << n;
        return std::vector<Message>{};
    }

    return nodes[message.to()]->onMessageReceive(message);
}
