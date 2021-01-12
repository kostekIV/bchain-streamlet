#include "node/honest_node.hpp"

unsigned HonestNode::EPOCH_LENGTH(2);

const Block& HonestNode::castFromHashable(const std::reference_wrapper<const Hashable>& ref){
    return *dynamic_cast<const Block *>(&ref.get());
}

bool HonestNode::finalizationPredicate(const std::vector<std::reference_wrapper<const Hashable>>& blocks){
    size_t len = blocks.size();
    if (len < 3) return false;
    int a = castFromHashable(blocks[len - 3]).epoch;
    int b = castFromHashable(blocks[len - 2]).epoch;
    int c = castFromHashable(blocks[len - 1]).epoch;
    return a + 1 == b and b + 1 == c;
}

HonestNode::HonestNode(unsigned id, unsigned numOfNodes, HashService service, Block genesisBlock):
    id(id),
    numOfNodes(numOfNodes),
    service(service), tree(genesisBlock, finalizationPredicate) {}

std::vector<Message> HonestNode::onMessageReceive(const Message& message){
    if(message.to() != id)
        return std::vector<Message>();
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
            return std::vector<Message>();;
    }
}

std::vector<Message> HonestNode::atTime(unsigned t){
    unsigned epoch = t/EPOCH_LENGTH+1;
    if(!t%EPOCH_LENGTH || service.getLeader(epoch) != id)
        return std::vector<Message>();
    Block block{castFromHashable(tree.getDeepestNotarized()).hash(), epoch, ""};
    return broadcast({MessageType::PROPOSAL, block});
}

std::vector<Message> HonestNode::broadcast(Content content){
    std::vector<Message> messages;
    messages.reserve(numOfNodes-1);
    for(unsigned i = 0; i < numOfNodes; i++)
        if(i != id)
            messages.push_back({id, i, content});
    return messages;
}