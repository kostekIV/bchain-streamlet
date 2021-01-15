#pragma once

#include <vector>
#include <memory>

#include "ischeduler.hpp"
#include "node/node.hpp"

class BaseScheduler : public IScheduler {
public:
    virtual std::vector<std::unique_ptr<INode>> takeOverNodes() override;

    virtual ~BaseScheduler() override = 0;

protected:
    explicit BaseScheduler(std::vector<std::unique_ptr<INode>>& nodes);

    virtual bool validMessageEndpoints(const Message& message) const;

    virtual std::vector<Message> sendRec(const Message& message);

    const unsigned n;
    unsigned timeSinceStart{0u};
    std::vector<std::unique_ptr<INode>> nodes;
};
