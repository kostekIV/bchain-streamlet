#include "catch2.hpp"

#include "node/dummy_node.hpp"
#include "node/honest_node.hpp"
#include "scenarios/config.hpp"
#include "scenarios/scheduler_type.hpp"
#include "scenarios/runner.hpp"


namespace {
    template<typename T>
    int countOfType(std::vector<std::unique_ptr<INode>>& v) {
        int count = 0;
        for (auto &x: v) {
            if (dynamic_cast<T*>(x.get())) {
                count += 1;
            }
        }
        return count;
    }
}

TEST_CASE("Runner summary") {
    ScenarioConfig config{"scenarios/test-config.yml"};
    Runner runner{config};

    runner.summary();
}

TEST_CASE("Runner play") {
    ScenarioConfig config{"scenarios/test-config.yml"};
    Runner runner{config};

    auto ret = runner.play();
    // on new implementations adjust
    REQUIRE(ret.size() == 20);

    int dummyCount = countOfType<DummyNode>(ret);
    REQUIRE(dummyCount == 10);

    int honestCount = countOfType<HonestNode>(ret);
    REQUIRE(honestCount == 10);
}
