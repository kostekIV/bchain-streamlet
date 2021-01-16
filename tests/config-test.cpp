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
    REQUIRE(config.getRepeatLeaderNTimes() == 1);
    REQUIRE(config.getSynchronizeEveryN() == 1);
}

TEST_CASE("Config has correct values - with optional values") {
    ScenarioConfig config{"scenarios/test-config-with-repeat.yml"};
    
    REQUIRE(config.getRepeatLeaderNTimes() == 12);
    REQUIRE(config.getSynchronizeEveryN() == 10);
}
