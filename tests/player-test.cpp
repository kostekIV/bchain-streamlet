#include "catch2.hpp"

#include "node/dummy_node.hpp"
#include "scenarios/config.hpp"
#include "scenarios/scheduler_type.hpp"
#include "scenarios/player.hpp"


TEST_CASE("Player summary") {
    ScenarioConfig config{"scenarios/test-config.yml"};
    Player player{config};

    player.summary();
}

TEST_CASE("Player play") {
    ScenarioConfig config{"scenarios/test-config.yml"};
    Player player{config};

    auto ret = player.play();
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

