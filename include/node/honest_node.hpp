#pragma once

#include <vector>
#include <unordered_set>

#include "state/tree/tree.hpp"
#include "round_service.hpp"
#include "state/block.hpp"
#include "node/node.hpp"

class HonestNode: public INode {
public:
    HonestNode(unsigned id, unsigned numOfNodes, RoundService& service, const Block& genesisBlock);
    
    std::vector<Message> onMessageReceive(const Message& message) override;

    std::vector<Message> atTime(unsigned t) override;

    const Tree& getTree();

private:
    static const Block& castFromHashable(const std::reference_wrapper<const Hashable>& ref);

    static bool finalizationPredicate(const std::vector<std::reference_wrapper<const Hashable>>& blocks);

    std::vector<Message> broadcast(Content content);

    const Block& storeBlock(const Block& block);

    Tree tree;
    unsigned id;
    unsigned numOfNodes;
    RoundService& service;
    std::vector<hash_t> proposedBlocks;
    std::vector<std::unordered_set<int>> votes;
    std::vector<Block> blocks;
};
