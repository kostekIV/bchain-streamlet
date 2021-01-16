#pragma once

#include <vector>
#include <unordered_set>

#include "services/abstract_service.hpp"
#include "state/tree.hpp"
#include "state/block.hpp"
#include "node/node.hpp"

class DishonestNode : public INode {
public:
    DishonestNode(unsigned id, unsigned numOfNodes, const AbstractService& service, const Block& genesisBlock, std::unordered_set<unsigned> dishonestNodes);

    std::vector<Message> onMessageReceive(const Message& message) override;

    std::vector<Message> atTime(unsigned t) override;

private:
    std::vector<Message> broadcast(const Content& content, const std::unordered_set<unsigned>& nodes);

    std::vector<Message> broadcastAll(const Content& content);

    Tree firstTree;
    Tree secondTree;
    bool firstHalf;
    const unsigned id;
    const unsigned numOfNodes;
    const AbstractService& service;
    std::unordered_set<unsigned> dishonestNodes;
    std::unordered_set<unsigned> honestNodesFirst;
    std::unordered_set<unsigned> honestNodesSecond;
    std::unordered_map<unsigned, hash_t> proposedBlocks;
    std::unordered_map<unsigned, std::unordered_set<unsigned>> votes;
};
