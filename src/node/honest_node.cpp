#include "node/honest_node.hpp"

#include "logging/easylogging++.h"

bool HonestNode::finalizationPredicate(const std::vector<std::reference_wrapper<const Block>>& blocks) {
    size_t len = blocks.size();
    if (len < 3) return false;
    unsigned a = blocks[len - 3].get().getEpoch();
    unsigned b = blocks[len - 2].get().getEpoch();
    unsigned c = blocks[len - 1].get().getEpoch();
    return a + 1 == b and b + 1 == c;
}

HonestNode::HonestNode(unsigned id, unsigned numOfNodes, const AbstractService& service, const Block& genesisBlock):
        id(id),
        numOfNodes(numOfNodes),
        service(service),
        tree(genesisBlock, HonestNode::finalizationPredicate) {
    proposedBlocks.try_emplace(0, genesisBlock.hash());
}

const Tree& HonestNode::getTree() { return tree; }

std::vector<Message> HonestNode::onMessageReceive(const Message& message) {
    LOG(DEBUG) << "[HONEST NODE " << id << "]: " << "Received message from node " << message.from()
               << " with type " << typeToString(message.content().messageType) << " and hash "
               << message.content().block.hash();
    if (message.to() != id)
        return {};
    Block block = message.content().block;
    unsigned epoch = message.content().block.getEpoch();
    switch (message.content().messageType) {
        case MessageType::PROPOSAL:
            if (message.from() != service.getLeader(epoch) ||
                proposedBlocks.find(epoch) != proposedBlocks.end())
                return {};
            proposedBlocks.try_emplace(epoch, block.hash());
            tree.addBlock(block);
            if (tree.isDeepestNotarized(block.getParentHash()))
                return broadcast({MessageType::VOTE, block});
            break;
        case MessageType::VOTE:
            if (proposedBlocks.find(epoch) == proposedBlocks.end() || proposedBlocks[epoch] != block.hash() ||
                tree.getBlock(block.hash()).getStatus() != Status::PRESENT)
                return {};
            votes[epoch].insert(message.from());
            if (3 * votes[epoch].size() >= 2 * numOfNodes) {
                LOG(DEBUG) << "[HONEST NODE " << id << "]: " << "Notarizing block with hash " << block.hash();
                tree.notarize(block);
            }
            break;
    }
    return {};
}

std::vector<Message> HonestNode::atTime(unsigned t) {
    LOG(DEBUG) << "[HONEST NODE " << id << "]: " << "Received ping at time " << t;
    unsigned epoch = service.getEpoch(t);
    if (!service.isEpochStart(t) || service.getLeader(epoch) != id)
        return {};
    const Block& parent = tree.getSomeDeepestNotarized();
    Block block{parent.hash(), epoch, service.getRandomPayload()};
    return broadcast({MessageType::PROPOSAL, block});
}

std::vector<Message> HonestNode::broadcast(const Content& content) {
    std::vector<Message> messages;
    messages.reserve(numOfNodes);
    for (unsigned i = 0; i < numOfNodes; i++)
        messages.emplace_back(id, i, content);
    return messages;
}
