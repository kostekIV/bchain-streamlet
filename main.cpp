#include "streamlet.hpp"
#include "logging/easylogging++.h"

INITIALIZE_EASYLOGGINGPP

int main(int argc, char **argv) {
    START_EASYLOGGINGPP(argc, argv);

    el::Configurations defaultConf;
    std::string enabled = "true";

    // to disable logging pass "false" as first argument
    if (argc > 1) {
        enabled = argv[1];
    }

    defaultConf.setGlobally(
                el::ConfigurationType::Enabled, enabled);
    el::Loggers::reconfigureLogger("default", defaultConf);

    DummyNode node;
    Message Message{0, 0, {MessageType::PROPOSAL, {"", 0, ""}}};
    return 0;
}
