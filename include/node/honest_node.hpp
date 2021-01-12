#pragma once

#include <vector>
#include <unordered_map>

#include "state/tree/tree.hpp"
#include "round_service.hpp"
#include "node/node.hpp"

class HonestNode: public INode {
private:
    using vote = std::pair<hash_t, std::pair<int, std::vector<bool>>>;
public:
    HonestNode(unsigned id, unsigned numOfNodes, RoundService& service, const Block& genesisBlock);
    
    std::vector<Message> onMessageReceive(const Message& message) override;

    std::vector<Message> atTime(unsigned t) override;

    const Tree& getTree();

private:
    static const Block& castFromHashable(const std::reference_wrapper<const Hashable>& ref);

    static bool finalizationPredicate(const std::vector<std::reference_wrapper<const Hashable>>& blocks);

    std::vector<Message> broadcast(Content content);

    Tree tree;
    unsigned id;
    unsigned numOfNodes;
    unsigned currentEpoch;
    RoundService& service;
    std::unordered_map<int, vote> votes;
};
