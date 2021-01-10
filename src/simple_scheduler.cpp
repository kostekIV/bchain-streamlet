#include "simple_scheduler.hpp"


namespace {
    template <typename T>
    bool check_range(T start, T end, T val) {
        return start <= val and val <= end;
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

void SimpleScheduler::clockTick() {
    broadcastTime();

    // Responding to the first messagess
    std::vector<Message> newMessages;
    while (!messages.empty()) {
        auto m = messages.front();
        messages.pop();
        auto ms = sendMessage(m);
        for (auto& x: ms) {
            messages.push(x);
        }
    }

    timeSinceStart++;
}

void SimpleScheduler::broadcastTime() {
    for (auto& n: nodes) {
        for (const auto& m: n->atTime(timeSinceStart)) {
            messages.push(m);
        }
    }
}


std::vector<Message> SimpleScheduler::sendMessage(const Message& message) {
    unsigned n = nodes.size();
    if (check_range(0u, n - 1, message.from()) or check_range(0u, n - 1, message.to())) {
        // ToDo add logging
        return std::vector<Message>{};
    }

    return nodes[message.to()]->onMessageReceive(message);
}
