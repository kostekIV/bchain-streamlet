#include <algorithm>
#include <utility>

#include "logging/easylogging++.h"

#include "state/tree.hpp"

#define range(V) (V).begin(), (V).end()

namespace {
    bool checkAndLogError(const std::unordered_map<hash_t, const Block>& hbMapping, const hash_t& key) {
        if (hbMapping.find(key) == hbMapping.end()) {
            LOG(ERROR) << "No key " << key << " in tree map";
            return false;
        }
        return true;
    }
}

Tree::Tree(const Block& rootContent, pred_t finalizationPredicate) :
        finalizationPredicate(std::move(finalizationPredicate)),
        rootHash(std::move(rootContent.hash())),
        hbMapping{{rootHash, rootContent}},
        depths{{rootHash, 0}},
        deepestNotarized(rootHash),
        deepestFinalized(rootHash) {
    auto& root = hbMapping.at(rootHash);
    root.notarize();
    root.finalize();

    LOG(DEBUG) << "[TREE]: " << "Initialised with \"" << rootHash << "\" as root hash";
}

bool Tree::isRoot(const Block& block) const { return depths.at(block.hash()) == 0; }

const Block& Tree::getSomeDeepestNotarized() const {
    LOG(DEBUG) << "[TREE]: " << "getSomeDeepestNotarized";
    return hbMapping.at(deepestNotarized);
}

void Tree::addBlock(const Block& child) {
    hash_t parentHash = child.getParentHash(), childHash = child.hash();
    LOG(DEBUG) << "[TREE]: " << "addBlock with hash " << childHash;
    checkAndLogError(hbMapping, parentHash);
    const Block& parentBlock = hbMapping.at(parentHash);
    hbMapping.try_emplace(childHash, child);
    depths.try_emplace(childHash, depths.at(parentHash) + 1);
}

void Tree::notarize(const Block& block) { notarize(block.hash()); }

void Tree::notarize(const hash_t& hash) {
    LOG(DEBUG) << "[TREE]: " << "notarize block with hash " << hash;
    checkAndLogError(hbMapping, hash);
    const Block& block = hbMapping.at(hash);
    block.notarize();
    if (depths.at(hash) > depths.at(deepestNotarized))
        deepestNotarized = hash;
    tryFinalizeUntil(block);
}

void Tree::tryFinalizeUntil(const Block& block) {
    LOG(DEBUG) << "[TREE]: " << "tryFinalizeUntil vertex with hash " << block.hash();
    blocks_t path = getPathFromRootTo(block);
    if (finalizationPredicate(path)) {
        for (unsigned i = path.size() - 2; i >= 0; i--) {
            if (path[i].get().getStatus() == Status::FINALIZED) {
                break;
            }
            path[i].get().finalize();
        }
        deepestFinalized = (path.end() - 2)->get().hash();
    }
}

Tree::blocks_t Tree::getPathFromRootTo(const Block& block) const {
    std::reference_wrapper<const Block> current = std::cref(block);
    blocks_t path = {current};
    while (not isRoot(current)) {
        current = hbMapping.at(current.get().getParentHash());
        path.push_back(current);
    }
    std::reverse(range(path));
    return path;
}

Tree::blocks_t Tree::getFinalizedChain() const {
    return getPathFromRootTo(hbMapping.at(deepestFinalized));
}

bool Tree::isDeepestNotarized(const Block& block) const { return isDeepestNotarized(block.hash()); }

bool Tree::isDeepestNotarized(const hash_t& hash) const {
    LOG(DEBUG) << "[TREE]: " << "check whether " << hash << " is one of the deepest notarized";
    if (!checkAndLogError(hbMapping, hash))
        return false;
    const auto& block = hbMapping.at(hash);
    return block.getStatus() != Status::PRESENT and depths.at(hash) == depths.at(deepestNotarized);
}

bool Tree::isPresent(const hash_t& hash) const {
    return hbMapping.find(hash) != hbMapping.end();
}

const Block& Tree::getBlock(const hash_t& hash) const {
    return hbMapping.at(hash);
}
