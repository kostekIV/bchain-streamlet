#include "catch2.hpp"

#include "node/dummy_node.hpp"
#include "node/honest_node.hpp"
#include "node/dishonest_node.hpp"
#include "scenarios/config.hpp"
#include "scenarios/scheduler_type.hpp"
#include "scenarios/runner.hpp"


namespace {
    template<typename T>
    int countOfType(std::vector<std::unique_ptr<INode>>& v) {
        return std::count_if(v.begin(), v.end(), [&](std::unique_ptr<INode>& x) {
            return dynamic_cast<T*>(x.get());
        });
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
    
    REQUIRE(ret.size() == 30);

    int dummyCount = countOfType<DummyNode>(ret);
    REQUIRE(dummyCount == 10);

    int honestCount = countOfType<HonestNode>(ret);
    REQUIRE(honestCount == 10);

    int dishonestCount = countOfType<DishonestNode>(ret);
    REQUIRE(dishonestCount == 10);
}
