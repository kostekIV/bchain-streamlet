#include "catch2.hpp"

#include <array>
#include <iostream>

#include "state/tree.hpp"
#include "state/state_renderer.hpp"

#define range(V) (V).begin(), (V).end()

bool finalizationPredicate(const std::vector<std::reference_wrapper<const Block>>& blocks) {
    size_t len = blocks.size();
    if (len < 3) return false;
    unsigned a = blocks[len - 3].get().getEpoch();
    unsigned b = blocks[len - 2].get().getEpoch();
    unsigned c = blocks[len - 1].get().getEpoch();
    return a + 1 == b and b + 1 == c;
}

struct TreeTestFixture {
    TreeTestFixture() {
        genHash = genesisBlock.hash();

        blocks.emplace_back(genesisBlock.hash(), 1, "");
        for (unsigned i = 0; i < 5; ++i) {
            blocks.emplace_back(blocks[i].hash(), i + 2, "");
        }

        blocks2.emplace_back(genesisBlock.hash(), 7, "");
        for (unsigned i = 0; i < 5; ++i) {
            blocks2.emplace_back(blocks2[i].hash(), i + 6 + 2, "");
        }
    }

    void createPath(int len, int beginBlock = 0) {
        tree.addBlock(blocks[beginBlock]);
        for (int i = beginBlock + 1; i < beginBlock + len; ++i)
            tree.addBlock(blocks[i]);
    }

    void createPath2(int len, int beginBlock = 0) {
        tree.addBlock(blocks2[beginBlock]);
        for (int i = beginBlock + 1; i < beginBlock + len; ++i)
            tree.addBlock(blocks2[i]);
    }

    void createNotarizedPath(int len, int beginBlock = 0) {
        createPath(len, beginBlock);
        for (int i = beginBlock; i < beginBlock + len; ++i)
            tree.notarize(blocks[i]);
    }

    void createNotarizedPath2(int len, int beginBlock = 0) {
        createPath2(len, beginBlock);
        for (int i = beginBlock; i < beginBlock + len; ++i)
            tree.notarize(blocks2[i]);
    }

    Block genesisBlock{"", 0, ""};
    hash_t genHash;
    std::vector<Block> blocks, blocks2;
    Tree tree{genesisBlock, finalizationPredicate};
};


TEST_CASE_METHOD(TreeTestFixture, "instantiation") {
    REQUIRE(true);
}

TEST_CASE_METHOD(TreeTestFixture, "creation") {
    SECTION("single node") {
        REQUIRE(tree.getSomeDeepestNotarized().hash() == genHash);
        REQUIRE(tree.isDeepestNotarized(genHash));
        REQUIRE(tree.isDeepestNotarized(genesisBlock));
    }

    SECTION("star") {
        for (unsigned i = 0; i < 5; ++i)
            tree.addBlock(Block{genHash, i + 1, ""});
        REQUIRE(tree.getSomeDeepestNotarized().hash() == genHash);
    }

    SECTION("path1") {
        for (int i = 0; i < 5; ++i) {
            tree.addBlock(blocks[i]);
            tree.notarize(blocks[i]);
            const Block& leaf = tree.getSomeDeepestNotarized();
            REQUIRE(leaf.hash() == blocks[i].hash());
        }
        REQUIRE(tree.isDeepestNotarized(blocks[4].hash()));
        REQUIRE(tree.isDeepestNotarized(blocks[4]));
    }

    SECTION("path2") {
        createPath(4);
        REQUIRE(tree.getSomeDeepestNotarized().hash() == genHash);
    }

    SECTION("path3") {
        createNotarizedPath(5);
        REQUIRE(tree.getSomeDeepestNotarized().hash() == blocks[4].hash());
        REQUIRE(tree.isDeepestNotarized(tree.getSomeDeepestNotarized()));
    }

    SECTION("longer chain") {
        createNotarizedPath(3);
        createNotarizedPath2(4);
        REQUIRE(tree.getSomeDeepestNotarized().hash() == blocks2[3].hash());
    }

    SECTION("parallel chains") {
        createNotarizedPath(3);
        createNotarizedPath2(3);
        REQUIRE(tree.isDeepestNotarized(blocks[2].hash()));
        REQUIRE(tree.isDeepestNotarized(blocks2[2].hash()));
    }
}

TEST_CASE_METHOD(TreeTestFixture, "finalization") {
    SECTION("root is finalized") {
        REQUIRE(tree.getFinalizedChain().size() == 1);
    }

    SECTION("no finalization (length)") {
        createPath(1);
        REQUIRE(tree.getFinalizedChain().size() == 1);

        tree.notarize(blocks[0]);
        REQUIRE(tree.getFinalizedChain().size() == 1);
    }

    SECTION("no finalization (predicate)") {
        Block b1{genHash, 2, ""}, b2{b1.hash(), 3, ""};

        tree.addBlock(b1);
        tree.addBlock(b2);

        tree.notarize(b1);
        tree.notarize(b2);
        REQUIRE(tree.getFinalizedChain().size() == 1);
    }

    SECTION("finalization (simple)") {
        createPath(4);
        for (int i = 0; i < 4; ++i)
            tree.notarize(blocks[i]);
        auto chain = tree.getFinalizedChain();

        REQUIRE(chain.size() == 4);

        REQUIRE(chain.front().get().hash() == genHash);
        for (int i = 1; i < 4; ++i)
            REQUIRE(chain[i].get().hash() == blocks[i - 1].hash());
    }

    SECTION("finalization (predicate on suffix)") {
        Block b1{genHash, 1, ""}, b3{b1.hash(), 3, ""}, b4{b3.hash(), 4, ""}, b5{b4.hash(), 5, ""};

        tree.addBlock(b1);
        tree.notarize(b1);
        tree.addBlock(b3);
        tree.notarize(b3);
        tree.addBlock(b4);
        tree.notarize(b4);
        tree.addBlock(b5);
        tree.notarize(b5);

        REQUIRE(tree.getFinalizedChain().size() == 4);
    }
}

TEST_CASE_METHOD(TreeTestFixture, "rendering generation") {
    SECTION("rendering single tree") {
        createNotarizedPath(4);
        createPath(2, 4);
        StateRenderer renderer([](const Block& h) { return h.hash(); });
        std::cout << renderer.renderTree({tree, ""}) << std::endl;
    }

    SECTION("rendering forest") {
        createNotarizedPath(4);
        createPath(2, 4);

        TreeTestFixture sibling;
        sibling.createPath(2);
        sibling.createNotarizedPath2(3);
        StateRenderer renderer([](const Block& h) { return h.hash(); });
        auto description = renderer.renderForest(
                {{tree,         "Node1"},
                 {sibling.tree, "Node2"}}
        );
        std::cout << description << std::endl;
    }
}
