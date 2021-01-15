#pragma once

#include <memory>
#include <vector>

#include "node/node.hpp"
#include "message/message.hpp"
#include "scheduler.hpp"
#include "queue.tpp"


class PartitioningScheduler : public IScheduler {
public:
    explicit PartitioningScheduler(std::vector<std::unique_ptr<INode>>& nodes);

    void start(unsigned nrRounds) override;

    ~PartitioningScheduler() = default;

private:
    void commonClockTick();

    void clockTick();

    void broadcastTime();

    Queue<Message> messagesCommon, messagesMaj2Maj, messagesMaj2Min, messagesMin2Maj, messagesMin2Min;
};
