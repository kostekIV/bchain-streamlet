#pragma once

#include <string>

#include "scenarios/scheduler_type.hpp"

class ScenarioConfig {
public:
    ScenarioConfig(std::string configFile);

    std::string getScenarioName() const;

    unsigned getNrOfRounds() const;

    unsigned getHonestNodesCount() const;
    unsigned getDummyNodesCount() const;
    unsigned getBitchNodesCount() const;

    SchedulerType getSchedulerType() const;
private:
    std::string scenarioName;
    unsigned nrOfRounds;

    unsigned honestNodesCount;
    unsigned dummyNodesCount;
    unsigned bitchNodesCount;

    SchedulerType schedulerType;
};
