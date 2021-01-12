#include "node/honest_node.hpp"

unsigned HonestNode::EPOCH_LENGTH(2);


bool HonestNode::finalizationPredicate(const std::vector<std::reference_wrapper<const Hashable>>& blocks){
    size_t len = blocks.size();
    if (len < 3) return false;
    int a = Block::castFromHashable(blocks[len - 3]).epoch;
    int b = Block::castFromHashable(blocks[len - 2]).epoch;
    int c = Block::castFromHashable(blocks[len - 1]).epoch;
    return a + 1 == b and b + 1 == c;
}

HonestNode::HonestNode(unsigned id, unsigned numOfNodes, const RoundService& service, const Block& genesisBlock):
    id(id),
    numOfNodes(numOfNodes),
    service(service), 
    tree(genesisBlock, finalizationPredicate) {}

std::vector<Message> HonestNode::onMessageReceive(const Message& message){
    if(message.to() != id)
        return {};
    const Block block = message.content().block;
    switch (message.content().messageType){
        case MessageType::PROPOSAL:
            if(tree.getDeepestNotarized().hash() == block.parentHash){
                tree.addBelow(tree.getDeepestNotarized(), block);
                votes[block.hash()]++;
                return broadcast({MessageType::VOTE, block});
            }
        case MessageType::VOTE:
            votes[block.hash()]++;
            if(3*votes[block.hash()] >= 2*numOfNodes)
                tree.notarize(block);
        default:
            return {};
    }
}

std::vector<Message> HonestNode::atTime(unsigned t){
    unsigned epoch = t/EPOCH_LENGTH+1;
    if(!t%EPOCH_LENGTH || service.getLeader(epoch) != id)
        return {};
    Block block{Block::castFromHashable(tree.getDeepestNotarized()).hash(), epoch, ""};
    return broadcast({MessageType::PROPOSAL, block});
}

std::vector<Message> HonestNode::broadcast(Content content){
    std::vector<Message> messages;
    messages.reserve(numOfNodes-1);
    for(unsigned i = 0; i < numOfNodes; i++)
            messages.push_back({id, i, content});
    return messages;
}
