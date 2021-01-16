#pragma once

#include <vector>
#include <unordered_set>
#include <unordered_map>

#include "services/abstract_service.hpp"
#include "state/tree.hpp"
#include "state/block.hpp"
#include "node/node.hpp"

class HonestNode : public INode {
public:
    HonestNode(unsigned id, unsigned numOfNodes, const AbstractService& service, const Block& genesisBlock);

    std::vector<Message> onMessageReceive(const Message& message) override;

    std::vector<Message> atTime(unsigned t) override;

    const Tree& getTree();

private:
    std::vector<Message> broadcast(const Content& content);

    Tree tree;
    const unsigned id;
    const unsigned numOfNodes;
    const AbstractService& service;
    std::unordered_map<unsigned, hash_t> proposedBlocks;
    std::unordered_map<unsigned, std::unordered_set<int>> votes;
};
