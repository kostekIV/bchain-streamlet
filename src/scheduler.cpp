#include "scheduler.hpp"


namespace {
    template <typename T>
    bool check_range(T start, T end, T val) {
        return start <= val and val <= end;
    }
}

Scheduler::Scheduler(std::vector<std::unique_ptr<INode>>& nodes):
    nodes(std::move(nodes)),
    timeSinceStart(0) {}

void Scheduler::start(unsigned nrRounds) {
    for (unsigned i = 0; i < nrRounds; i++) {
        clockTick();
    }
}

void Scheduler::clockTick() {
    broadcastTime();

    // Responding to the first messagess
    std::vector<Message> newMessages;
    while (!messages.empty()) {
        auto m = messages.front();
        messages.pop();
        auto ms = sendMessage(m);
        newMessages.insert(newMessages.end(), ms.begin(), ms.end());
    }

    // Responding to responses
    for (const auto& m: newMessages) {
        for(auto const& response: sendMessage(m)) {
            messages.push(response);
        }
    }

    timeSinceStart++;
}

void Scheduler::broadcastTime() {
    for (auto& n: nodes) {
        for (const auto& m: n.get()->atTime(timeSinceStart)) {
            messages.push(m);
        }
    }
}


std::vector<Message> Scheduler::sendMessage(const Message& message) {
    unsigned n = nodes.size();
    if (check_range(0u, n - 1, message.from()) or check_range(0u, n - 1, message.to())) {
        // ToDo add logging
        return std::vector<Message>{};
    }

    return nodes[message.to()].get()->onMessageReceive(message);
}
