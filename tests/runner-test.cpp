#include "catch2.hpp"

#include "node/dummy_node.hpp"
#include "scenarios/config.hpp"
#include "scenarios/scheduler_type.hpp"
#include "scenarios/runner.hpp"


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
    REQUIRE(ret.size() == 10);

    unsigned dummyCount = 0;
    for (auto &x: ret) {
        if (dynamic_cast<DummyNode*>(x.get())) {
            dummyCount += 1;
        }
    }
    REQUIRE(dummyCount == 10);
}

