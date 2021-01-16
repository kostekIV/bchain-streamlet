#pragma once

#include <vector>
#include <functional>
#include <unordered_map>
#include "block.hpp"

class Tree {
private:
    using blocks_t = std::vector<std::reference_wrapper<const Block>>;
    using hashes_t = std::vector<hash_t>;
    using pred_t = std::function<bool(blocks_t)>;

public:
    Tree(const Block& rootContent, pred_t finalizePredicate);

    const Block& getSomeDeepestNotarized() const;

    bool isDeepestNotarized(const Block& block) const;

    bool isDeepestNotarized(const hash_t& hash) const;

    void addBlock(const Block& child);

    void notarize(const Block& Block);

    void notarize(const hash_t& hash);

    blocks_t getFinalizedChain() const;

    bool isPresent(const hash_t& hash) const;

    const Block& getBlock(const hash_t& hash) const;

private:
    void tryFinalizeUntil(const Block& block);

    blocks_t getPathFromRootTo(const Block& v) const;

    bool isRoot(const Block& block) const;

    const pred_t finalizationPredicate;
    const hash_t rootHash;
    hash_t deepestNotarized, deepestFinalized;
    std::unordered_map<hash_t, const Block> hbMapping;
    std::unordered_map<hash_t, unsigned> depths;

    friend class StateRenderer;
};
