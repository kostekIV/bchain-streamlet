#include "state/block.hpp"

#include <utility>

Block::Block(hash_t parentHash, unsigned epoch, std::string payload) :
        parentHash(std::move(parentHash)), epoch(epoch), payload(std::move(payload)) {}

hash_t Block::hash() const {
    return parentHash + std::to_string(epoch) + payload;
}

const Block& Block::castFromHashable(const std::reference_wrapper<const Hashable>& ref){
    return *dynamic_cast<const Block *>(&ref.get());
}

Block Block::createGenesisBlock() {
    return Block("", 0, "genesis_block");
}
