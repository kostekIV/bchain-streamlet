#include "node/honest_node.hpp"

HonestNode::HonestNode(unsigned id, unsigned numOfNodes, const RoundService& service, const Block& genesisBlock):
    id(id),
    numOfNodes(numOfNodes),
    service(service), 
    currentEpoch(0),
    tree(genesisBlock, RoundService::finalizationPredicate) {}

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
    if(!service.isNewEpoch(t) || service.getLeader(t) != id)
        return {};
    unsigned epoch = service.getEpoch(t);
    Block block{Block::castFromHashable(tree.getSomeDeepestNotarized()).hash(), epoch, service.getRandomPayload()};
    return broadcast({MessageType::PROPOSAL, block});
}

std::vector<Message> HonestNode::broadcast(Content content){
    std::vector<Message> messages;
    messages.reserve(numOfNodes-1);
    for(unsigned i = 0; i < numOfNodes; i++)
            messages.push_back({id, i, content});
    return messages;
}
