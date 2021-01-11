#include "catch2.hpp"

#include <array>

#include "node/hashable.hpp"
#include "node/tree/tree.hpp"

#define range(V) (V).begin(), (V).end()

struct TestHashable : Hashable {
    int id;

    TestHashable(int id) : id(id) {}

    TestHashable& operator=(const TestHashable&) = delete;

    TestHashable(const TestHashable&) = delete;

    hash_t hash() const override { return std::to_string(id); }
};

const TestHashable& castFromHashable(const std::reference_wrapper<const Hashable>& ref) {
    return *dynamic_cast<const TestHashable *>(&ref.get());
}

bool finalizationPredicate(const std::vector<std::reference_wrapper<const Hashable>>& blocks) {
    size_t len = blocks.size();
    if (len < 3) return false;
    int a = castFromHashable(blocks[len - 3]).id;
    int b = castFromHashable(blocks[len - 2]).id;
    int c = castFromHashable(blocks[len - 1]).id;
    return a + 1 == b and b + 1 == c;
}

struct TreeTestFixture {
    void createPath(int len, int beginBlock = 0) {
        tree.addBelow(genesisBlock, blocks[beginBlock]);
        for (int i = beginBlock + 1; i < beginBlock + len; ++i)
            tree.addBelow(blocks[i - 1], blocks[i]);
    }

    void createNotarizedPath(int len, int beginBlock = 0) {
        createPath(len, beginBlock);
        for (int i = beginBlock; i < beginBlock + len; ++i)
            tree.notarize(blocks[i]);
    }

    TestHashable genesisBlock{-1};
    std::array<TestHashable, 8> blocks = {0, 1, 2, 3, 4, 5, 6, 7};
    Tree tree{genesisBlock, finalizationPredicate};
};


TEST_CASE_METHOD(TreeTestFixture, "instantiation") {
    REQUIRE(true);
}

TEST_CASE_METHOD(TreeTestFixture, "creation") {
    SECTION("single node") {
        REQUIRE(tree.getDeepestNotarized().hash() == "-1");
    }

    SECTION("star") {
        for (int i = 0; i < 5; ++i)
            tree.addBelow(genesisBlock, blocks.back());
        REQUIRE(tree.getDeepestNotarized().hash() == "-1");
    }

    SECTION("path1") {
        for (int i = 0; i < 5; ++i) {
            const TestHashable& leaf = (const TestHashable&) tree.getDeepestNotarized();
            REQUIRE(leaf.hash() == std::to_string(i - 1));
            tree.addBelow(leaf, blocks[i]);
            tree.notarize(blocks[i]);
        }
    }

    SECTION("path2") {
        createPath(6);
        REQUIRE(tree.getDeepestNotarized().hash() == "-1");
    }

    SECTION("path3") {
        createNotarizedPath(6);
        REQUIRE(tree.getDeepestNotarized().hash() == "5");
    }

    SECTION("longer chain") {
        createNotarizedPath(3);
        createNotarizedPath(4, 3);
        REQUIRE(tree.getDeepestNotarized().hash() == "6");
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
        tree.addBelow(genesisBlock, blocks[2]);
        tree.addBelow(blocks[2], blocks[3]);

        tree.notarize(blocks[2]);
        tree.notarize(blocks[3]);
        REQUIRE(tree.getFinalizedChain().size() == 1);
    }

    SECTION("finalization (simple)") {
        createPath(4);
        for (int i = 0; i < 4; ++i)
            tree.notarize(blocks[i]);
        auto chain = tree.getFinalizedChain();

        REQUIRE(chain.size() == 4);

        for (int i = 0; i < 4; ++i)
            REQUIRE(chain[i].get().hash() == std::to_string(i - 1));
    }

    SECTION("finalization (predicate on suffix)") {
        tree.addBelow(genesisBlock, blocks[1]);
        tree.notarize(blocks[1]);
        tree.addBelow(blocks[1], blocks[3]);
        tree.notarize(blocks[3]);
        tree.addBelow(blocks[3], blocks[4]);
        tree.notarize(blocks[4]);
        tree.addBelow(blocks[4], blocks[5]);
        tree.notarize(blocks[5]);

        REQUIRE(tree.getFinalizedChain().size() == 4);
    }
}
