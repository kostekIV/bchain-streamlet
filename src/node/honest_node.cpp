#include "node/honest_node.hpp"

HonestNode::HonestNode(unsigned id, unsigned numOfNodes, RoundService& service, const Block& genesisBlock):
    id(id),
    numOfNodes(numOfNodes),
    service(service), 
    currentEpoch(0),
    tree(genesisBlock, RoundService::finalizationPredicate) {}

std::vector<Message> HonestNode::onMessageReceive(const Message& message){
    if(message.to() != id)
        return {};
    const Block& block = message.content().block;
    unsigned epoch = message.content().block.epoch;
    switch (message.content().messageType){
        case MessageType::PROPOSAL:
            if(message.from() != service.getLeader(epoch))
                return {};
            if(votes.find(epoch) != votes.end())
                return {};
            votes[epoch] = {block.hash(), {0, std::vector<bool>(numOfNodes, false)}};
            if(tree.isDeepestNotarized(block.parentHash)){
                tree.addBelow(block.parentHash, block);
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
            if(3*v.second.first >= 2*numOfNodes)
                tree.notarize(block);
            break;
    }
    return {};
}

std::vector<Message> HonestNode::atTime(unsigned t){
    unsigned epoch = service.getEpoch(t);
    if(!service.isNewEpoch(t) || service.getLeader(epoch) != id)
        return {};
    Block parent = Block::castFromHashable(tree.getSomeDeepestNotarized());
    Block block = service.allocateBlock({parent.hash(), epoch, service.getRandomPayload()});
    return broadcast({MessageType::PROPOSAL, block});
}

std::vector<Message> HonestNode::broadcast(Content content){
    std::vector<Message> messages;
    messages.reserve(numOfNodes - 1);
    for(unsigned i = 0; i < numOfNodes; i++)
            messages.push_back({id, i, content});
    return messages;
}
