#pragma once

#include <vector>
#include <unordered_set>

#include "services/abstract_service.hpp"
#include "state/tree.hpp"
#include "state/block.hpp"
#include "node/base_node.hpp"

class DishonestNode : public BaseNode {
public:
    DishonestNode(unsigned id, unsigned numOfNodes, const AbstractService& service, const Block& genesisBlock, std::unordered_set<unsigned> dishonestNodes);

    std::vector<Message> onMessageReceive(const Message& message) override;

    std::vector<Message> atTime(unsigned t) override;

private:
    std::vector<Message> broadcast(const Content& content, const std::unordered_set<unsigned>& nodes);

    std::vector<Message> broadcastAll(const Content& content);

    std::vector<Message> handlePropose(const Message& message, bool mischief, unsigned k);

    std::vector<Message> preparePropose(unsigned epoch, unsigned k);

    void handleVote(const Message& message, unsigned k);

    Tree trees[2];
    bool firstHalf;
    const unsigned id;
    const unsigned numOfNodes;
    const AbstractService& service;
    std::unordered_set<unsigned> allNodes;
    std::unordered_set<unsigned> dishonestNodes;
    std::unordered_set<unsigned> honestNodesFractions[2];
    std::unordered_map<unsigned, hash_t> proposedBlocks[2];
    std::unordered_map<unsigned, std::unordered_set<unsigned>> votes[2];
};
