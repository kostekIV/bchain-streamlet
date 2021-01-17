#pragma once

#include <memory>
#include <vector>
#include <unordered_set>

#include "node/node.hpp"
#include "message/message.hpp"
#include "schedulers/base_scheduler.hpp"
#include "schedulers/queue.hpp"


class ISynchronizationDecider {
public:
    virtual bool shouldSynchronize(unsigned round) = 0;
    virtual ~ISynchronizationDecider() = 0;
};

class PartitioningScheduler : public BaseScheduler {
public:
    explicit PartitioningScheduler(std::vector<std::unique_ptr<INode>>& nodes,
                                   std::unique_ptr<ISynchronizationDecider>& decider);

    void start(unsigned nrRounds) override;

    ~PartitioningScheduler() = default;

private:
    enum EdgeType {
        MAJ_TO_MAJ,
        MAJ_TO_MIN,
        MIN_TO_MAJ,
        MIN_TO_MIN
    };

    class Action: public IQueueAction<std::pair<unsigned, Message>> {
    public:
        Action(PartitioningScheduler& scheduler);

        void onPop(std::pair<unsigned, Message> rm) override;
    private:
        PartitioningScheduler& scheduler;
    };

    void initialize();

    void redirectMessagesToQueue(std::vector<Message>&& messages, unsigned round);

    void clockTick();

    void broadcastTime();

    void beforeAndAfter();

    EdgeType getEdgeType(const Message& m);

    Action action;
    std::unique_ptr<ISynchronizationDecider> decider;
    std::unordered_set<unsigned> minorityIds;
    std::vector<std::pair<unsigned, Message>> messages;
    Queue<std::pair<unsigned, Message>> messagesMaj2Maj, messagesMaj2Min, messagesMin2Maj, messagesMin2Min;
};
