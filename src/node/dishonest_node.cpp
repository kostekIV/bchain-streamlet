#include "node/dishonest_node.hpp"
#include "utils.hpp"

#include "logging/easylogging++.h"

DishonestNode::DishonestNode(unsigned id, unsigned numOfNodes, const AbstractService& service, 
    const Block& genesisBlock, std::unordered_set<unsigned> dishonestNodes):
        id(id),
        firstHalf(true),
        numOfNodes(numOfNodes),
        service(service),
        trees{{genesisBlock, DishonestNode::finalizationPredicate},
        {genesisBlock, DishonestNode::finalizationPredicate}},
        proposedBlocks{{{0, genesisBlock.hash()}}, {{0, genesisBlock.hash()}}},
        dishonestNodes(dishonestNodes) {
    for (unsigned i = 0; i < numOfNodes; i++) {
        allNodes.insert(i);
    }

    unsigned numOfHonest = numOfNodes - dishonestNodes.size();

    for (unsigned i = 0; i < numOfNodes; i++) {
        if (dishonestNodes.find(i) == dishonestNodes.end()) {
            if (honestNodesFractions[0].size() < numOfHonest / 2) {
                honestNodesFractions[0].insert(i);
            }
            else {
                honestNodesFractions[1].insert(i);
            }
        }
    }
}

std::vector<Message> DishonestNode::onMessageReceive(const Message& message) {
    LOG(DEBUG) << "[DISHONEST NODE " << id << "]: " << "Received message from node " << message.from()
               << " with type " << typeToString(message.content().messageType) << " and hash "
               << message.content().block.hash();

    if (message.to() != id) {
        return {};
    }
    
    std::vector<Message> messages1, messages2;

    bool mischief = message.content().messageType == MessageType::PROPOSAL and
        dishonestNodes.find(message.from()) != dishonestNodes.end();

    if (mischief) {
        LOG(DEBUG) << "[DISHONEST NODE " << id << "]: " << "Entering mischief mode";
    }
    
    switch (message.content().messageType) {
        case MessageType::PROPOSAL:
            messages1 = handlePropose(message, mischief, 0);
            messages2 = handlePropose(message, mischief, 1);
            break;
        case MessageType::VOTE:
            handleVote(message, 0);
            handleVote(message, 1);
            break;
    }

    if (mischief) {
        firstHalf = !firstHalf;
    }
    return messages1.size() > 0 ? messages1 : messages2;
}

std::vector<Message> DishonestNode::atTime(unsigned t) {
    LOG(DEBUG) << "[DISHONEST NODE " << id << "]: " << "Received ping at time " << t;

    unsigned epoch = service.getEpoch(t);

    if (!service.isEpochStart(t) || service.getLeader(epoch) != id) {
        return {};
    }

    auto messages = preparePropose(epoch, 0);
    utils::insert(messages, preparePropose(epoch, 1));
    
    return messages;
}

std::vector<Message> DishonestNode::preparePropose(unsigned epoch, unsigned k) {
    const Block& parent = trees[k].getSomeDeepestNotarized();

    Block block{parent.hash(), epoch, service.getRandomPayload()};

    auto messages = broadcast({MessageType::PROPOSAL, block}, honestNodesFractions[k]);
    utils::insert(messages, broadcast({MessageType::PROPOSAL, block}, dishonestNodes));
    
    return messages;
}

std::vector<Message> DishonestNode::broadcast(const Content& content, const std::unordered_set<unsigned>& nodes) {
    std::vector<Message> messages;
    messages.reserve(nodes.size());

    for (auto i : nodes) {
        messages.emplace_back(id, i, content);
    }
    return messages;
}

std::vector<Message> DishonestNode::broadcastAll(const Content& content) {
    return broadcast(content, allNodes);
}

std::vector<Message> DishonestNode::handlePropose(const Message& message, bool mischief, unsigned k) {
    Block block = message.content().block;
    unsigned epoch = block.getEpoch();
    std::vector<Message> messages;

    if (message.from() != service.getLeader(epoch) || proposedBlocks[k].find(epoch) != proposedBlocks[k].end()) {
        return {};
    }

    if (!mischief || !firstHalf == k) {
        proposedBlocks[k].try_emplace(epoch, block.hash());

        if (trees[k].isPresent(block.getParentHash())) {
            trees[k].addBlock(block);
        }
        if (trees[k].isDeepestNotarized(block.getParentHash())) {
            if (!mischief) {
                messages = broadcastAll({MessageType::VOTE, block});
            } else {
                messages = broadcast({MessageType::VOTE, block}, honestNodesFractions[k]);
                utils::insert(messages, broadcast({MessageType::VOTE, block}, dishonestNodes));
            }
        }
    }
    return messages;
}

void DishonestNode::handleVote(const Message& message, unsigned k) {
    Block block = message.content().block;
    unsigned epoch = block.getEpoch();

    if (proposedBlocks[k].find(epoch) == proposedBlocks[k].end() || proposedBlocks[k][epoch] != block.hash() ||
        !trees[k].isPresent(block.hash()) || trees[k].getBlock(block.hash()).getStatus() != Status::PRESENT) {
        return;
    }
    
    votes[k][epoch].insert(message.from());
    if (3 * votes[k][epoch].size() > 2 * numOfNodes) {
        LOG(DEBUG) << "[DISHONEST NODE " << id << "]: " << "Notarizing block with hash " << block.hash();

        trees[k].notarize(block);
    }
}
