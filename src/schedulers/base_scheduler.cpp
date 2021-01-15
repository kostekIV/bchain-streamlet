#include "schedulers/base_scheduler.hpp"
#include "logging/easylogging++.h"

BaseScheduler::BaseScheduler(std::vector<std::unique_ptr<INode>>& nodes) :
        n(nodes.size()),
        nodes(std::move(nodes)) {}

BaseScheduler::~BaseScheduler() {}

std::vector<std::unique_ptr<INode>> BaseScheduler::takeOverNodes() { return std::move(this->nodes); }

bool BaseScheduler::validMessageEndpoints(const Message& message) const {
    return 0u <= message.from() and message.from() < n and
           0u <= message.to() and message.to() < n;
}

std::vector<Message> BaseScheduler::sendRec(const Message& message) {
    if (not validMessageEndpoints(message)) {
        LOG(ERROR) << "Message receiver or sender not it range\n" << "Receiver: " << message.to() << ", Sender: "
                   << message.from() << ", Node count: " << n;
        return {};
    }
    return nodes[message.to()]->onMessageReceive(message);
}
