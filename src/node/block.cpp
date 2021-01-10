#include "node/block.hpp"

#include <utility>

Block::Block(hash_t parent_hash, uint epoch, std::string payload) :
        parent_hash(std::move(parent_hash)), epoch(epoch), payload(std::move(payload)) {}

hash_t Block::hash() const {
    // TODO
    return hash_t();
}
