#include "catch2.hpp"

#include "scenarios/config.hpp"
#include "scenarios/scheduler_type.hpp"


TEST_CASE("Config has correct values") {
    ScenarioConfig config{"scenarios/test-config.yml"};

    REQUIRE(config.getScenarioName() == "Test Config");
    REQUIRE(config.getNrOfRounds() == 10);
    REQUIRE(config.getHonestNodesCount() == 10);
    REQUIRE(config.getDummyNodesCount() == 10);
    REQUIRE(config.getDishonestNodesCount() == 10);
    REQUIRE(config.getSchedulerType() == SchedulerType::SIMPLE);
    REQUIRE(config.getServiceType() == ServiceType::NORMAL);
}
