#include <vector>
#include <utility>
#include <string>

#include "streamlet.hpp"
#include "logging/easylogging++.h"

INITIALIZE_EASYLOGGINGPP

int main(int argc, char **argv) {
    START_EASYLOGGINGPP(argc, argv);

    el::Configurations defaultConf;
    std::string enabled = "true";
    std::string path = "./scenarios/honest_with_dummies.yml";

    // scenarion path is passed as first argument
    if (argc > 1) {
        path = argv[1];
    }
    // to disable logging pass "false" as second argument
    if (argc > 2) {
        enabled = argv[2];
    }

    defaultConf.setGlobally(
                el::ConfigurationType::Enabled, enabled);
    el::Loggers::reconfigureLogger("default", defaultConf);

    ScenarioConfig config{path};
    std::cout << "Config done\n";
    Runner runner{config};
    std::cout << "Runner init done\n";

    runner.summary();
    auto ret = runner.play();

    std::vector<std::pair<const Tree&, std::string>> forest;

    int i = 0;
    for (const auto& x: ret) {
        HonestNode* node = dynamic_cast<HonestNode*>(x.get());
        if (node) {
            i += 1;
            forest.emplace_back(node->getTree(), std::to_string(i));
        }
    }

    StateRenderer stateRenderer([](const Block& b) { return std::to_string(b.getEpoch()); });
    std::cout << stateRenderer.renderForest(forest);

    return 0;
}
