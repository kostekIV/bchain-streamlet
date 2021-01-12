#pragma once

#include <string>

#include "scenarios/scheduler_type.hpp"

class ScenarioConfig {
public:
    explicit ScenarioConfig(std::string configFile);

    std::string getScenarioName() const;

    unsigned getNrOfRounds() const;

    unsigned getHonestNodesCount() const;
    unsigned getDummyNodesCount() const;
    unsigned getDishonestNodesCount() const;

    SchedulerType getSchedulerType() const;
private:
    std::string scenarioName;
    unsigned nrOfRounds;

    unsigned honestNodesCount;
    unsigned dummyNodesCount;
    unsigned dishonestNodesCount;

    SchedulerType schedulerType;
};
