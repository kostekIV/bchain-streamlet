#include <vector>
#include <memory>

#include "logging/easylogging++.h"

#include "scenarios/runner.hpp"

#include "services/abstract_service.hpp"
#include "services/repeat_service.hpp"
#include "services/synchronizer.hpp"
#include "node/node.hpp"
#include "node/dummy_node.hpp"
#include "node/honest_node.hpp"
#include "node/dishonest_node.hpp"
#include "schedulers/ischeduler.hpp"
#include "schedulers/simple_scheduler.hpp"
#include "schedulers/partitioning_scheduler.hpp"
#include "state/block.hpp"

namespace {
    std::unique_ptr<IScheduler> getScheduler(SchedulerType type, std::vector<std::unique_ptr<INode>>& nodes,
                                             unsigned synchronizeEveryN) {
        if (type == SchedulerType::SIMPLE) {
            return std::make_unique<SimpleScheduler>(nodes);
        } else if (type == SchedulerType::PARTITIONING) {
            std::unique_ptr<ISynchronizationDecider> decider = std::make_unique<Synchronizer>(synchronizeEveryN);
            return std::make_unique<PartitioningScheduler>(nodes, decider);
        }
        throw std::runtime_error("Unknown scheduler type is unsupported");
    }

    std::unique_ptr<AbstractService> getService(ServiceType type, unsigned n, unsigned epochLen, unsigned repeatTimes) {
        switch (type) {
            case ServiceType::REPEATING_LEADER:
                return std::make_unique<RepeatService>(n, epochLen, repeatTimes);
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
    serviceType(config.getServiceType()),
    repeatLeaderNTimes(config.getRepeatLeaderNTimes()),
    synchronizeEveryN(config.getSynchronizeEveryN()),
    epochLenght(config.getEpochLenght()) {}

std::string Runner::summary() {
    std::ostringstream summary;
    summary << "Scenario Name: " << scenarioName << std::endl;
    summary << "Scenario will play for " << nrOfRounds << " rounds" << std::endl;
    summary << "Scenario info:" << std::endl;
    summary << "* Scheduler used: " << schedulerType << std::endl;
    summary << "* Round Service used: " << serviceType << std::endl;
    summary << "* Honest nodes count: " << honestNodesCount << std::endl;
    summary << "* Dummy nodes count: " << dummyNodesCount << std::endl;
    summary << "* Dishonest nodes count: " << dishonestNodesCount << std::endl;
    summary << "* Repeat leader?: " << repeatLeaderNTimes << std::endl;
    summary << "* Synchronize every?: " << synchronizeEveryN << std::endl;
    summary << "* Epoch lenght?: " << epochLenght << std::endl;

    return summary.str();
}

std::vector<std::unique_ptr<INode>> Runner::play() {
    std::vector<std::unique_ptr<INode>> allNodes;

    unsigned n = honestNodesCount + dummyNodesCount + dishonestNodesCount;
    std::unique_ptr<AbstractService> service = getService(serviceType, n, epochLenght, repeatLeaderNTimes);
    Block genesisBlock = Block::createGenesisBlock();

    for (unsigned i = 0; i < honestNodesCount; i++) {
        allNodes.emplace_back(std::make_unique<HonestNode>(i, n, *service, genesisBlock));
    }
    for (unsigned i = 0; i < dummyNodesCount; i++) {
        allNodes.emplace_back(std::make_unique<DummyNode>());
    }
    std::unordered_set<unsigned> dishonestNodes;
    for (unsigned i = honestNodesCount + dummyNodesCount; i < n; i++) {
        dishonestNodes.insert(i);
    }
    for (unsigned i = honestNodesCount + dummyNodesCount; i < n; i++) {
        allNodes.emplace_back(std::make_unique<DishonestNode>(i, n, *service, genesisBlock, dishonestNodes));
    }
    std::unique_ptr<IScheduler> scheduler = getScheduler(schedulerType, allNodes, synchronizeEveryN);

    // End of scenario initialization

    LOG(INFO) << "Runner started the scenario: " << scenarioName;
    scheduler->start(nrOfRounds);
    LOG(INFO) << "Scenario: " << scenarioName << " finished";

    return scheduler->takeOverNodes();
}
