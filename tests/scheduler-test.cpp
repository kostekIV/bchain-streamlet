#include "catch2.hpp"

#include <vector>
#include <memory>

#include "message.hpp"
#include "node.hpp"
#include "simple_scheduler.hpp"

struct TestNode: public INode {
public:
    std::vector<Message> onMessageReceive(const Message& message) override {
        return {};
    }
    std::vector<Message> atTime(unsigned t) override {
        return {Message{0, 0, {}}};
    }
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
