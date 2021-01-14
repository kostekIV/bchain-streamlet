#pragma once

#include <memory>
#include <vector>
#include <queue>

#include "node/node.hpp"
#include "message/message.hpp"
#include "scheduler.hpp"


class PartitioningScheduler : public IScheduler {
public:
    explicit PartitioningScheduler(std::vector<std::unique_ptr<INode>>& nodes);

    void start(unsigned nrRounds) override;

    std::vector<std::unique_ptr<INode>> takeOverNodes() override;

    ~PartitioningScheduler() = default;

private:
    unsigned timeSinceStart;
    std::vector<std::unique_ptr<INode>> nodes;
    std::queue<Message> messagesCommon, messagesMaj2Maj, messagesMaj2Min, messagesMin2Maj, messagesMin2Min;

    void commonClockTick();

    void clockTick();

    std::vector<Message> sendRec(const Message& message);

    void broadcastTime();
};
