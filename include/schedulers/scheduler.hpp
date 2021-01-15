#pragma once

#include <vector>
#include <memory>

#include "node/node.hpp"

class IScheduler {
public:
    virtual void start(unsigned nrRounds) = 0;

    /**
     * It is important to note that this function gives up ownership of nodes
     * that the scheduler is holding.
     */
    virtual std::vector<std::unique_ptr<INode>> takeOverNodes();

    virtual ~IScheduler() = 0;

protected:
    explicit IScheduler(std::vector<std::unique_ptr<INode>>& nodes);

    virtual bool validMessageEndpoints(const Message& message) const;

    virtual std::vector<Message> sendRec(const Message& message);

    const unsigned n;
    unsigned timeSinceStart{0u};
    std::vector<std::unique_ptr<INode>> nodes;
};
