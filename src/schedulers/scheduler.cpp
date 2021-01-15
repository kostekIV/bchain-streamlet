#include "schedulers/scheduler.hpp"
#include "logging/easylogging++.h"

IScheduler::IScheduler(std::vector<std::unique_ptr<INode>>& nodes) :
        n(nodes.size()),
        nodes(std::move(nodes)) {}

IScheduler::~IScheduler() {}

std::vector<std::unique_ptr<INode>> IScheduler::takeOverNodes() { return std::move(this->nodes); }

bool IScheduler::validMessageEndpoints(const Message& message) const {
    return 0u <= message.from() and message.from() < n and
           0u <= message.to() and message.to() < n;
}

std::vector<Message> IScheduler::sendRec(const Message& message) {
    if (not validMessageEndpoints(message)) {
        LOG(ERROR) << "Message receiver or sender not it range\n" << "Receiver: " << message.to() << ", Sender: "
                   << message.from() << ", Node count: " << n;
        return std::vector<Message>{};
    }
    return nodes[message.to()]->onMessageReceive(message);
}
