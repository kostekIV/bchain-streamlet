#include "node/honest_node.hpp"

#include "logging/easylogging++.h"

bool HonestNode::finalizationPredicate(const std::vector<std::reference_wrapper<const Hashable>>& blocks) {
    size_t len = blocks.size();
    if (len < 3) return false;
    int a = Block::castFromHashable(blocks[len - 3]).epoch;
    int b = Block::castFromHashable(blocks[len - 2]).epoch;
    int c = Block::castFromHashable(blocks[len - 1]).epoch;
    return a + 1 == b and b + 1 == c;
}

HonestNode::HonestNode(unsigned id, unsigned numOfNodes, RoundService& service, const Block& genesisBlock):
    id(id),
    numOfNodes(numOfNodes),
    service(service),
    tree(genesisBlock, HonestNode::finalizationPredicate) {}

const Block& HonestNode::storeBlock(const Block& block) {
    blocks.push_back(block);
    return blocks.back();
}

std::vector<Message> HonestNode::onMessageReceive(const Message& message) {
    LOG(DEBUG) << "[HONEST NODE " << id << "]: " << "Received message from node " << message.from() 
        << " with type " << typeToString(message.content().messageType) << " and hash " << message.content().block.hash();
    if(message.to() != id)
        return {};
    Block block = message.content().block;
    unsigned epoch = message.content().block.epoch;
    switch (message.content().messageType){
        case MessageType::PROPOSAL:
            if(message.from() != service.getLeader(epoch))
                return {};
            if(votes.find(epoch) != votes.end())
                return {};
            votes[epoch] = {block.hash(), {0, std::vector<bool>(numOfNodes, false)}};
            if(tree.isDeepestNotarized(block.parentHash)) {
                tree.addBelow(block.parentHash, storeBlock(block));
                return broadcast({MessageType::VOTE, block});
            }
            break;
        case MessageType::VOTE:
            if(votes.find(epoch) == votes.end())
                return {};
            vote& v = votes[epoch];
            if(v.first != block.hash())
                return {};
            if(v.second.second[message.from()])
                return {};
            v.second.first++;
            if(3 * v.second.first >= 2 * numOfNodes) {
                LOG(DEBUG) << "[HONEST NODE " << id << "]: " << "Notarizing block with hash " << block.hash();
                tree.notarize(block);
            }
            break;
    }
    return {};
}

std::vector<Message> HonestNode::atTime(unsigned t){
    LOG(DEBUG) << "[HONEST NODE " << id << "]: " << "Received ping at time " << t;
    unsigned epoch = service.getEpoch(t);
    if(!service.isEpochStart(t) || service.getLeader(epoch) != id)
        return {};
    Block parent = Block::castFromHashable(tree.getSomeDeepestNotarized());
    Block block{parent.hash(), epoch, service.getRandomPayload()};
    return broadcast({MessageType::PROPOSAL, block});
}

std::vector<Message> HonestNode::broadcast(Content content){
    std::vector<Message> messages;
    messages.reserve(numOfNodes - 1);
    for(unsigned i = 0; i < numOfNodes; i++)
        messages.push_back({id, i, content});
    return messages;
}
