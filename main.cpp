#include "streamlet.hpp"
#include "logging/easylogging++.h"

INITIALIZE_EASYLOGGINGPP

int main(int argc, char **argv) {
    START_EASYLOGGINGPP(argc, argv);
    LOG(INFO) << "My first info log using default logger";

    DummyNode node;
    Message Message{0, 0, {MessageType::PROPOSAL, {"", 0, ""}}};
    return 0;
}
