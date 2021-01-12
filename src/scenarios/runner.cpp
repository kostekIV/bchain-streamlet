
#include <iostream>
#include <sstream>
#include <vector>
#include <memory>

#include "scenarios/runner.hpp"

#include "node/node.hpp"
#include "node/dummy_node.hpp"
#include "scheduler/scheduler.hpp"
#include "scheduler/simple_scheduler.hpp"

namespace {
    std::unique_ptr<IScheduler> get_scheduler(SchedulerType type, std::vector<std::unique_ptr<INode>>& nodes) {
        if (type == SchedulerType::SIMPLE) {
            return std::make_unique<SimpleScheduler>(nodes);
        }
        throw std::runtime_error("Unknown scheduler type is unsupported");
    }
}

Runner::Runner(const ScenarioConfig& config):
    scenarioName(config.getScenarioName()),
    nrOfRounds(config.getNrOfRounds()),
    honestNodesCount(config.getHonestNodesCount()),
    dummyNodesCount(config.getDummyNodesCount()),
    dishonestNodesCount(config.getDishonestNodesCount()),
    schedulerType(config.getSchedulerType()) {}

void Runner::summary() {
    std::ostringstream summary;
    summary << "Scenario Name: " << scenarioName << std::endl;
    summary << "Scenario will play for " << nrOfRounds << " rounds" << std::endl;
    summary << "Scenario info:" << std::endl;
    summary << "* Scheduler used: " << schedulerType << std::endl;
    summary << "* Honest nodes count: " << honestNodesCount << std::endl;
    summary << "* Dummy nodes count:  " << dummyNodesCount << std::endl;
    summary << "* Dishonest nodes count:  " << dishonestNodesCount << std::endl;

    std::cout << summary.str();
}

std::vector<std::unique_ptr<INode>> Runner::play() {
    std::vector<std::unique_ptr<INode>> allNodes;

    for (unsigned i = 0; i < honestNodesCount; i++) {
        // ToDo initialize
    }
    for (unsigned i = 0; i < dummyNodesCount; i++) {
        allNodes.emplace_back(std::make_unique<DummyNode>());
    }
    for (unsigned i = 0; i < dishonestNodesCount; i++) {
        // ToDo initialize
    }
    std::unique_ptr<IScheduler> scheduler = get_scheduler(schedulerType, allNodes);

    // End of scenario initialization
    scheduler->start(nrOfRounds);

    return scheduler->takeOverNodes();
}
