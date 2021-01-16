#include "catch2.hpp"

#include <vector>
#include <memory>

#include "logging/easylogging++.h"
#include "message/message.hpp"
#include "node/node.hpp"
#include "schedulers/simple_scheduler.hpp"
#include "schedulers/partitioning_scheduler.hpp"

struct TestNode: public INode {
    unsigned atTimeCount = 0;
    unsigned onMessageReceivedCount = 0;
    std::unordered_set<unsigned> getFrom;
    unsigned sendTo = 0;
    unsigned sendFrom = 0;

    std::vector<Message> onMessageReceive(const Message& message) override {
        this->onMessageReceivedCount += 1;

        getFrom.emplace(message.from());
        return {};
    }
    std::vector<Message> atTime(unsigned t) override {
        this->atTimeCount += 1;
        return {Message{sendFrom, sendTo, {MessageType::PROPOSAL, {"", 0, ""}}}};
    }

    ~TestNode() {}
};

struct TestSynchronizer: public ISynchronizationDecider {
    TestSynchronizer(unsigned synchronizeOn): synchronizeOn(synchronizeOn) {}

    bool shouldSynchronize(unsigned round) override {
        return synchronizeOn == round;
    }

    unsigned synchronizeOn;
};

TEST_CASE("Scheduler does not hang") {
    std::vector<std::unique_ptr<INode>> nodes;
    for (unsigned i = 0; i < 10; i++) {
      nodes.emplace_back(std::make_unique<TestNode>());
    }
    SimpleScheduler sched(nodes);
    sched.start(10000);

    REQUIRE(true);
}

TEST_CASE("`atTime` is called for each round") {
    std::vector<std::unique_ptr<INode>> nodes;
    for (unsigned i = 0; i < 10; i++) {
      nodes.emplace_back(std::make_unique<TestNode>());
    }
    SimpleScheduler sched(nodes);
    sched.start(1000);

    for (auto& x: sched.getNodes()) {
        REQUIRE(dynamic_cast<TestNode*>(x.get())->atTimeCount == 1000);
    }
}

TEST_CASE("`onMessageReceived` is called appropriate amount of time") {
    std::vector<std::unique_ptr<INode>> nodes;
    for (unsigned i = 0; i < 10; i++) {
      nodes.emplace_back(std::make_unique<TestNode>());
    }
    SimpleScheduler sched(nodes);
    sched.start(1000);

    int i = 0;
    for (auto& x: sched.getNodes()) {
        if (i == 0) {
            REQUIRE(dynamic_cast<TestNode*>(x.get())->onMessageReceivedCount == 10 * 1000);
        } else {
            REQUIRE(dynamic_cast<TestNode*>(x.get())->onMessageReceivedCount == 0);
        }
        i++;
    }
}

TEST_CASE("Partitioning scheduler does not hang - without synchronization") {
    std::vector<std::unique_ptr<INode>> nodes;
    std::unique_ptr<ISynchronizationDecider> decider = std::make_unique<TestSynchronizer>(-1);
    for (unsigned i = 0; i < 10; i++) {
      nodes.emplace_back(std::make_unique<TestNode>());
    }
    PartitioningScheduler sched(nodes, decider);
    sched.start(500);

    REQUIRE(true);
}

TEST_CASE("Partitioning scheduler does not hang - with synchronization") {
    std::vector<std::unique_ptr<INode>> nodes;
    std::unique_ptr<ISynchronizationDecider> decider = std::make_unique<TestSynchronizer>(20);
    for (unsigned i = 0; i < 10; i++) {
      nodes.emplace_back(std::make_unique<TestNode>());
    }
    PartitioningScheduler sched(nodes, decider);
    sched.start(500);

    REQUIRE(true);
}


TEST_CASE("Partitioning scheduler does not send messages between partitions") {
    std::vector<std::unique_ptr<INode>> nodes;
    std::unique_ptr<ISynchronizationDecider> decider = std::make_unique<TestSynchronizer>(-1);
    for (unsigned i = 0; i < 10; i++) {
      nodes.emplace_back(std::make_unique<TestNode>());
    }

    auto* nodeFromMinority = dynamic_cast<TestNode*>(nodes[0].get());
    auto* nodeFromMajority = dynamic_cast<TestNode*>(nodes[9].get());
    
    nodeFromMinority->sendTo = 9;
    nodeFromMinority->sendFrom = 0;
    
    nodeFromMajority->sendFrom = 9;
    nodeFromMajority->sendTo = 0;

    PartitioningScheduler sched(nodes, decider);
    sched.start(100);

    nodes = sched.takeOverNodes();

    nodeFromMinority = dynamic_cast<TestNode*>(nodes[0].get());
    nodeFromMajority = dynamic_cast<TestNode*>(nodes[9].get());

    REQUIRE(nodeFromMinority->getFrom.find(9) == nodeFromMinority->getFrom.end());
    REQUIRE(nodeFromMajority->getFrom.find(0) == nodeFromMajority->getFrom.end());
}

TEST_CASE("Partitioning scheduler does send messages between partitions after synchronization") {
    std::vector<std::unique_ptr<INode>> nodes;
    std::unique_ptr<ISynchronizationDecider> decider = std::make_unique<TestSynchronizer>(20);
    for (unsigned i = 0; i < 10; i++) {
      nodes.emplace_back(std::make_unique<TestNode>());
    }

    auto* nodeFromMinority = dynamic_cast<TestNode*>(nodes[0].get());
    auto* nodeFromMajority = dynamic_cast<TestNode*>(nodes[9].get());
    
    nodeFromMinority->sendTo = 9;
    nodeFromMinority->sendFrom = 0;
    
    nodeFromMajority->sendFrom = 9;
    nodeFromMajority->sendTo = 0;

    PartitioningScheduler sched(nodes, decider);
    sched.start(100);

    nodes = sched.takeOverNodes();

    nodeFromMinority = dynamic_cast<TestNode*>(nodes[0].get());
    nodeFromMajority = dynamic_cast<TestNode*>(nodes[9].get());

    REQUIRE(nodeFromMinority->getFrom.find(9) != nodeFromMinority->getFrom.end());
    REQUIRE(nodeFromMajority->getFrom.find(0) != nodeFromMajority->getFrom.end());
}
