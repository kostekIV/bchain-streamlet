#pragma once

#include <memory>
#include <vector>
#include <unordered_set>

#include "node/node.hpp"
#include "message/message.hpp"
#include "schedulers/base_scheduler.hpp"
#include "schedulers/queue.hpp"


class PartitioningScheduler : public BaseScheduler, public IQueueAction<Message> {
public:
    explicit PartitioningScheduler(std::vector<std::unique_ptr<INode>>& nodes);

    void start(unsigned nrRounds) override;

    void onPop(Message m) override;

    ~PartitioningScheduler() = default;

private:
    void initialize();

    void commonClockTick();

    void clockTick();

    void broadcastTime();

    std::unordered_set<unsigned> majorityIds;
    std::unordered_set<unsigned> minorityIds;
    Queue<Message> messagesCommon, messagesMaj2Maj, messagesMaj2Min, messagesMin2Maj, messagesMin2Min;
};
