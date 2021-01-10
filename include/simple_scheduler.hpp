#pragma once

#include <memory>
#include <vector>
#include <queue>

#include "node.hpp"
#include "message.hpp"
#include "scheduler.hpp"


class SimpleScheduler: public IScheduler {
public:
    SimpleScheduler(std::vector<std::unique_ptr<INode>>& nodes);
    void start(unsigned nrRounds) override;
private:
    unsigned timeSinceStart;
    std::vector<std::unique_ptr<INode>> nodes;
    std::queue<Message> messages;

    void clockTick();
    std::vector<Message> sendMessage(const Message& message);
    void broadcastTime();
};