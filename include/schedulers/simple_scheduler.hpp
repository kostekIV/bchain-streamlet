#pragma once

#include <memory>
#include <vector>

#include "node/node.hpp"
#include "message/message.hpp"
#include "scheduler.hpp"
#include "queue.tpp"


class SimpleScheduler : public IScheduler {
public:
    explicit SimpleScheduler(std::vector<std::unique_ptr<INode>>& nodes);

    void start(unsigned nrRounds) override;

    const std::vector<std::unique_ptr<INode>>& getNodes() const;

    ~SimpleScheduler() = default;

private:
    void clockTick();

    void broadcastTime();

    Queue<Message> messages;
};
