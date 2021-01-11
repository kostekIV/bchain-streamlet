#include <yaml-cpp/yaml.h>

#include "scenarios/config.hpp"

namespace {
    SchedulerType get_scheduler(std::string type) {
        if (type == "simple") {
            return SchedulerType::SIMPLE;
        }
        throw std::runtime_error("Unknown scheduler type is unsupported");
    }
}

ScenarioConfig::ScenarioConfig(std::string configFile) {
    YAML::Node config = YAML::LoadFile(configFile);

    scenarioName = config["scenario-name"].as<std::string>();

    nrOfRounds = config["rounds"].as<unsigned>();

    honestNodesCount = config["honest-nodes"].as<unsigned>();
    dummyNodesCount = config["dummy-nodes"].as<unsigned>();
    bitchNodesCount = config["bitch-nodes"].as<unsigned>();

    schedulerType = get_scheduler(config["scheduler-type"].as<std::string>());
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
unsigned ScenarioConfig::getBitchNodesCount() const {
    return bitchNodesCount;
}

SchedulerType ScenarioConfig::getSchedulerType() const {
    return schedulerType;
}
