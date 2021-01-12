#include "catch2.hpp"

#include <vector>
#include <memory>

#include "message/message.hpp"
#include "node/node.hpp"
#include "scheduler/simple_scheduler.hpp"

struct TestNode: public INode {
public:
    unsigned atTimeCount = 0;
    unsigned onMessageReceivedCount = 0;
    std::vector<Message> onMessageReceive(const Message& message) override {
        this->onMessageReceivedCount += 1;
        return {};
    }
    std::vector<Message> atTime(unsigned t) override {
        this->atTimeCount += 1;
        return {Message{0, 0, {MessageType::PROPOSAL, {"", 0, ""}}}};
    }

    ~TestNode() {}
};

TEST_CASE("Scheduler does not hangs") {
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
