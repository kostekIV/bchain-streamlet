#include "state/block.hpp"

#include <utility>

Block::Block(hash_t parentHash, unsigned epoch, std::string payload) :
        parentHash(std::move(parentHash)), epoch(epoch), payload(std::move(payload)) {}

hash_t Block::hash() const {
    return std::to_string(epoch);
}
