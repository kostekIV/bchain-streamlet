#include <yaml-cpp/yaml.h>

#include "scenarios/config.hpp"

namespace {
    SchedulerType getScheduler(std::string type) {
        if (type == "simple") {
            return SchedulerType::SIMPLE;
        } else if (type == "[artitioning") {
            return SchedulerType::PARTITIONING;
        }
        throw std::runtime_error("Unknown scheduler type is unsupported");
    }

    ServiceType getService(std::string type) {
        if (type == "normal") {
            return ServiceType::NORMAL;
        }
        throw std::runtime_error("Unknown service type is unsupported");
    }
}

ScenarioConfig::ScenarioConfig(std::string configFile) {
    YAML::Node config = YAML::LoadFile(configFile);

    scenarioName = config["scenario-name"].as<std::string>();

    nrOfRounds = config["rounds"].as<unsigned>();

    honestNodesCount = config["honest-nodes"].as<unsigned>();
    dummyNodesCount = config["dummy-nodes"].as<unsigned>();
    dishonestNodesCount = config["dishonest-nodes"].as<unsigned>();

    schedulerType = getScheduler(config["scheduler-type"].as<std::string>());
    serviceType = getService(config["service-type"].as<std::string>());

    repeatLeaderNTimes = config["repeat-n-times"] ? config["repeat-n-times"].as<unsigned>() : 1;
    synchronizeEveryN = config["synchronize-every-n"] ? config["synchronize-every-n"].as<unsigned>() : 1;
}

std::string ScenarioConfig::getScenarioName() const {
    return scenarioName;
}

unsigned ScenarioConfig::getNrOfRounds() const {
    return nrOfRounds;
}

unsigned ScenarioConfig::getHonestNodesCount() const {
    return honestNodesCount;
}
unsigned ScenarioConfig::getDummyNodesCount() const {
    return dummyNodesCount;
}
unsigned ScenarioConfig::getDishonestNodesCount() const {
    return dishonestNodesCount;
}

SchedulerType ScenarioConfig::getSchedulerType() const {
    return schedulerType;
}

ServiceType ScenarioConfig::getServiceType() const {
    return serviceType;
}

unsigned ScenarioConfig::getRepeatLeaderNTimes() const {
    return repeatLeaderNTimes;
}


unsigned ScenarioConfig::getSynchronizeEveryN() const {
    return synchronizeEveryN;
}
