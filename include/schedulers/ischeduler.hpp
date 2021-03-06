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
    virtual std::vector<std::unique_ptr<INode>> takeOverNodes() = 0;

    virtual ~IScheduler() = 0;
};
