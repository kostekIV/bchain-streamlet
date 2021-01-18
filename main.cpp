#include <vector>
#include <utility>
#include <string>

#include "streamlet.hpp"
#include "logging/easylogging++.h"

INITIALIZE_EASYLOGGINGPP

int main(int argc, char **argv) {
    START_EASYLOGGINGPP(argc, argv);

    el::Configurations defaultConf;
    std::string enabled = "false";
    std::string path = "./scenarios/honest_with_dummies.yml";

    // to disable/enable logging pass "false/true" as first argument
    if (argc > 1) {
        enabled = argv[1];
    }
    // scenario path is passed as second argument
    if (argc > 2) {
        path = argv[2];
    }

    defaultConf.setGlobally(
                el::ConfigurationType::Enabled, enabled);
    el::Loggers::reconfigureLogger("default", defaultConf);

    ScenarioConfig config{path};
    Runner runner{config};

    std::cerr << runner.summary();
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

    StateRenderer stateRenderer([](const Block& b) { return std::to_string(b.getEpoch()) + "\\n" + b.hash().substr(0, 5); });
    std::cout << stateRenderer.renderForest(forest);

    return 0;
}
