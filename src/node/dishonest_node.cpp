#include "node/dishonest_node.hpp"

#include "logging/easylogging++.h"

namespace {
    template <typename T>
    void insert(std::vector<T>& dest, std::vector<T>&& src) {
        for (auto& el: src) {
            dest.push_back(el);
        }
    }
}

DishonestNode::DishonestNode(unsigned id, unsigned numOfNodes, const AbstractService& service, const Block& genesisBlock, std::unordered_set<unsigned> dishonestNodes):
        id(id),
        firstHalf(true),
        numOfNodes(numOfNodes),
        service(service),
        firstTree(genesisBlock, DishonestNode::finalizationPredicate),
        secondTree(genesisBlock, DishonestNode::finalizationPredicate),
        dishonestNodes(dishonestNodes) {
    proposedBlocks.try_emplace(0, genesisBlock.hash());
    unsigned numOfHonest = numOfNodes - dishonestNodes.size();
    for (unsigned i = 0; i < numOfNodes; i++)
        if (dishonestNodes.find(i) == dishonestNodes.end())
            if (honestNodesFirst.size() < numOfHonest / 2)
                honestNodesFirst.insert(i);
            else
                honestNodesSecond.insert(i);
}

std::vector<Message> DishonestNode::onMessageReceive(const Message& message) {
    LOG(DEBUG) << "[DISHONEST NODE " << id << "]: " << "Received message from node " << message.from()
               << " with type " << typeToString(message.content().messageType) << " and hash "
               << message.content().block.hash();
    if (message.to() != id)
        return {};
    Block block = message.content().block;
    unsigned epoch = message.content().block.getEpoch();
    bool mischief = message.content().messageType == MessageType::PROPOSAL and
        dishonestNodes.find(message.from()) != dishonestNodes.end();
    if(mischief)
        LOG(DEBUG) << "[DISHONEST NODE " << id << "]: " << "Entering mischief mode";
    std::vector<Message> messages;
    switch (message.content().messageType) {
        case MessageType::PROPOSAL:
            if (message.from() != service.getLeader(epoch) ||
                proposedBlocks.find(epoch) != proposedBlocks.end())
                return {};
            if (!mischief) {
                proposedBlocks.try_emplace(epoch, block.hash());
                if (firstTree.isPresent(block.getParentHash()))
                    firstTree.addBlock(block);
                if (secondTree.isPresent(block.getParentHash()))
                    secondTree.addBlock(block);
                if (firstTree.isDeepestNotarized(block.getParentHash()))
                    messages = broadcastAll({MessageType::VOTE, block});
            } else if (firstHalf) {
                firstTree.addBlock(block);
                firstTree.notarize(block);
                messages = broadcast({MessageType::VOTE, block}, honestNodesFirst);
            } else {
                secondTree.addBlock(block);
                secondTree.notarize(block);
                messages = broadcast({MessageType::VOTE, block}, honestNodesSecond);
            }
            break;
        case MessageType::VOTE:
            if (!mischief) {
                if (proposedBlocks.find(epoch) == proposedBlocks.end() || proposedBlocks[epoch] != block.hash() ||
                    !firstTree.isPresent(block.hash()) || firstTree.getBlock(block.hash()).getStatus() != Status::PRESENT)
                    return {};
                votes[epoch].insert(message.from());
                if (3 * votes[epoch].size() >= 2 * numOfNodes) {
                    LOG(DEBUG) << "[DISHONEST NODE " << id << "]: " << "Notarizing block with hash " << block.hash();
                    firstTree.notarize(block);
                    if(secondTree.isPresent(block.hash()))
                        secondTree.notarize(block);
                }
            }
            break;
    }
    if (mischief)
        firstHalf = !firstHalf;
    return messages;
}

std::vector<Message> DishonestNode::atTime(unsigned t) {
    LOG(DEBUG) << "[DISHONEST NODE " << id << "]: " << "Received ping at time " << t;
    unsigned epoch = service.getEpoch(t);
    if (!service.isEpochStart(t) || service.getLeader(epoch) != id)
        return {};
    const Block& parent1 = firstTree.getSomeDeepestNotarized();
    const Block& parent2 = secondTree.getSomeDeepestNotarized();
    Block block1{parent1.hash(), epoch, service.getRandomPayload()};
    Block block2{parent2.hash(), epoch, service.getRandomPayload()};
    auto messages = broadcast({MessageType::PROPOSAL, block1}, honestNodesFirst);
    insert(messages, broadcast({MessageType::PROPOSAL, block2}, honestNodesSecond));
    insert(messages, broadcast({MessageType::PROPOSAL, block1}, dishonestNodes));
    insert(messages, broadcast({MessageType::PROPOSAL, block2}, dishonestNodes));
    return messages;
}

std::vector<Message> DishonestNode::broadcast(const Content& content, const std::unordered_set<unsigned>& nodes) {
    std::vector<Message> messages;
    messages.reserve(nodes.size());
    for (auto i : nodes)
        messages.emplace_back(id, i, content);
    return messages;
}

std::vector<Message> DishonestNode::broadcastAll(const Content& content) {
    std::vector<Message> messages;
    messages.reserve(numOfNodes);
    for (unsigned i = 0; i < numOfNodes; i++)
        messages.emplace_back(id, i, content);
    return messages;
}
