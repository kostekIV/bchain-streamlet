#include <utility>
#include <functional>
#include <cassert>

#include "logging/easylogging++.h"

#include "state/block.hpp"

const hash_t Block::EMPTY_HASH;

Block::Block(hash_t parentHash, unsigned epoch, std::string payload) :
        parentHash(std::move(parentHash)), epoch(epoch), payload(std::move(payload)) {}

hash_t Block::hash() const {
    return std::to_string(std::hash<hash_t>{}(parentHash + std::to_string(epoch) + payload));
}

Block Block::createGenesisBlock() { return Block(EMPTY_HASH, 0, "genesis_block"); }

hash_t Block::getParentHash() const { return parentHash; }

unsigned Block::getEpoch() const { return epoch; }

std::string Block::getPayload() const { return payload; }

Status Block::getStatus() const { return status; }

void Block::notarize() const {
    LOG(DEBUG) << "[Vertex]: notarizing vertex with status " << statusToString(status);
    assert(status == Status::PRESENT);
    status = Status::NOTARIZED;
}

void Block::finalize() const {
    LOG(DEBUG) << "[Vertex]: finalizing vertex with status " << statusToString(status);
    assert(status == Status::NOTARIZED or status == Status::FINALIZED);
    status = Status::FINALIZED;
}
