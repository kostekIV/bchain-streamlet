#include "node/block.hpp"

#include <utility>

Block::Block(hash_t parent_hash, unsigned epoch, std::string payload) :
        parent_hash(std::move(parent_hash)), epoch(epoch), payload(std::move(payload)) {}

hash_t Block::hash() const {
    return std::to_string(epoch);
}
