#pragma once

#include <vector>
#include <memory>

#include "node/node.hpp"
#include "scenarios/config.hpp"
#include "scenarios/scheduler_type.hpp"
#include "scenarios/service_type.hpp"

class Runner {
public:
    Runner(const ScenarioConfig& config);
    std::vector<std::unique_ptr<INode>> play();
    void summary();
private:
    const std::string scenarioName;
    const unsigned nrOfRounds;

    const unsigned honestNodesCount;
    const unsigned dummyNodesCount;
    const unsigned dishonestNodesCount;

    const SchedulerType schedulerType;
    const ServiceType serviceType;

    const unsigned repeatLeaderNTimes;
    const unsigned synchronizeEveryN;
    const unsigned epochLenght;
};
