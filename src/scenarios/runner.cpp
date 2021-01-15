
#include <iostream>
#include <vector>
#include <memory>

#include "logging/easylogging++.h"

#include "scenarios/runner.hpp"

#include "service/round_service.hpp"
#include "service/twice_service.hpp"
#include "node/node.hpp"
#include "node/dummy_node.hpp"
#include "schedulers/ischeduler.hpp"
#include "schedulers/simple_scheduler.hpp"
#include "node/honest_node.hpp"
#include "state/block.hpp"

namespace {
    std::unique_ptr<IScheduler> getScheduler(SchedulerType type, std::vector<std::unique_ptr<INode>>& nodes) {
        if (type == SchedulerType::SIMPLE) {
            return std::make_unique<SimpleScheduler>(nodes);
        }
        throw std::runtime_error("Unknown scheduler type is unsupported");
    }

    std::unique_ptr<RoundService> getService(ServiceType type, unsigned n) {
        switch (type) {
            case ServiceType::NORMAL:
                return std::make_unique<RoundService>(n);
                break;
            case ServiceType::TWICE:
                return std::make_unique<TwiceService>(n);
                break;
            default:
                throw std::runtime_error("Unknown service type is unsupported");
        }
    }
}

Runner::Runner(const ScenarioConfig& config):
    scenarioName(config.getScenarioName()),
    nrOfRounds(config.getNrOfRounds()),
    honestNodesCount(config.getHonestNodesCount()),
    dummyNodesCount(config.getDummyNodesCount()),
    dishonestNodesCount(config.getDishonestNodesCount()),
    schedulerType(config.getSchedulerType()),
    serviceType(config.getServiceType()) {}

void Runner::summary() {
    std::ostringstream summary;
    summary << "Scenario Name: " << scenarioName << std::endl;
    summary << "Scenario will play for " << nrOfRounds << " rounds" << std::endl;
    summary << "Scenario info:" << std::endl;
    summary << "* Scheduler used: " << schedulerType << std::endl;
    summary << "* Round Serivce used: " << serviceType << std::endl;
    summary << "* Honest nodes count: " << honestNodesCount << std::endl;
    summary << "* Dummy nodes count:  " << dummyNodesCount << std::endl;
    summary << "* Dishonest nodes count:  " << dishonestNodesCount << std::endl;

    std::cout << summary.str();
}

std::vector<std::unique_ptr<INode>> Runner::play() {
    std::vector<std::unique_ptr<INode>> allNodes;

    unsigned n = honestNodesCount + dummyNodesCount + dishonestNodesCount;
    std::unique_ptr<RoundService> service = getService(serviceType, n);
    Block genesisBlock = Block::createGenesisBlock();

    for (unsigned i = 0; i < honestNodesCount; i++) {
        allNodes.emplace_back(std::make_unique<HonestNode>(i, n, *service, genesisBlock));
    }
    for (unsigned i = 0; i < dummyNodesCount; i++) {
        allNodes.emplace_back(std::make_unique<DummyNode>());
    }
    for (unsigned i = 0; i < dishonestNodesCount; i++) {
        // ToDo initialize
    }
    std::unique_ptr<IScheduler> scheduler = getScheduler(schedulerType, allNodes);

    // End of scenario initialization

    LOG(INFO) << "Runner started the scenario: " << scenarioName;
    scheduler->start(nrOfRounds);
    LOG(INFO) << "Scenario: " << scenarioName << " finished";

    return scheduler->takeOverNodes();
}
