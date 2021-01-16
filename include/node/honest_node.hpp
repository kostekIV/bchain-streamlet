#pragma once

#include <vector>
#include <list>
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
    static bool finalizationPredicate(const std::vector<std::reference_wrapper<const Block>>& blocks);

    std::vector<Message> broadcast(const Content& content);

    const Block& storeBlock(const Block& block);

    Tree tree;
    const unsigned id;
    const unsigned numOfNodes;
    const AbstractService& service;
    std::unordered_map<int, hash_t> proposedBlocks;
    std::unordered_set<hash_t> notarizedBlocks;
    std::unordered_map<int, std::unordered_set<int>> votes;
    std::list<Block> blocks;
};
