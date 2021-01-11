#include <node/block.hpp>
#include "catch2.hpp"

#define range(V) (V).begin(), (V).end()

struct TestHashable : Hashable {
    int id;

    TestHashable(int id) : id(id) {}

    TestHashable& operator=(const TestHashable&) = delete;
    TestHashable(const TestHashable&) = delete;

    hash_t hash() const override { return std::to_string(id); }
};

bool finalizationPredicate(const std::vector<const Hashable*>& blocks) {
    if (blocks.empty()) return false;
    std::vector<const TestHashable*> casted;
    std::for_each(range(blocks), [&casted](auto h) { casted.push_back(dynamic_cast<const TestHashable*>(h)); });
    size_t len = casted.size();
    int lastId = casted[len - 1]->id;
    return len >= 3 and
           casted[len - 2]->id == lastId - 1 and
           casted[len - 3]->id == lastId - 2;
}

struct DagTestFixture {
    DagTestFixture() {
        doug.notarize(genesisBlock);
    }

    void createPath(int len, int beginBlock = 0) {
        doug.addBelow(genesisBlock, blocks[beginBlock]);
        for (int i = beginBlock + 1; i < beginBlock + len; ++i)
            doug.addBelow(blocks[i - 1], blocks[i]);
    }

    void createNotarizedPath(int len, int beginBlock = 0) {
        createPath(len, beginBlock);
        for (int i = beginBlock; i < beginBlock + len; ++i)
            doug.notarize(blocks[i]);
    }

    TestHashable genesisBlock{-1};
    std::array<TestHashable, 8> blocks = {0, 1, 2, 3, 4, 5, 6, 7};
    Dag doug{genesisBlock, finalizationPredicate};
};


TEST_CASE_METHOD(DagTestFixture, "instantiation") {
    REQUIRE(true);
}

TEST_CASE_METHOD(DagTestFixture, "creation") {
    SECTION("single node") {
        REQUIRE(doug.getDeepestNotarized().hash() == "-1");
    }

    SECTION("star") {
        for (int i = 0; i < 5; ++i)
            doug.addBelow(genesisBlock, blocks.back());
        REQUIRE(doug.getDeepestNotarized().hash() == "-1");
    }

    SECTION("path1") {
        for (int i = 0; i < 5; ++i) {
            const TestHashable& leaf = (const TestHashable&) doug.getDeepestNotarized();
            REQUIRE(leaf.hash() == std::to_string(i - 1));
            doug.addBelow(leaf, blocks[i]);
            doug.notarize(blocks[i]);
        }
    }

    SECTION("path2") {
        createPath(6);
        REQUIRE(doug.getDeepestNotarized().hash() == "-1");
    }

    SECTION("path3") {
        createNotarizedPath(6);
        REQUIRE(doug.getDeepestNotarized().hash() == "5");
    }

    SECTION("longer chain") {
        createNotarizedPath(3);
        createNotarizedPath(4, 3);
        REQUIRE(doug.getDeepestNotarized().hash() == "6");
    }
}

TEST_CASE_METHOD(DagTestFixture, "finalization") {
    SECTION("no finalization (length)") {
        createPath(1);
        REQUIRE(doug.getFinalizedChain().empty());

        doug.notarize(blocks[0]);
        REQUIRE(doug.getFinalizedChain().empty());
    }

    SECTION("no finalization (predicate)") {
        doug.addBelow(genesisBlock, blocks[2]);
        doug.addBelow(blocks[2], blocks[3]);

        doug.notarize(blocks[2]);
        doug.notarize(blocks[3]);
        REQUIRE(doug.getFinalizedChain().empty());
    }

    SECTION("finalization (simple)") {
        createPath(4);
        for (int i = 0; i < 4; ++i)
            doug.notarize(blocks[i]);
        auto chain = doug.getFinalizedChain();

        REQUIRE(chain.size() == 4);

        for (int i = 0; i < 4; ++i)
            REQUIRE(chain[i]->hash() == std::to_string(i - 1));
    }

    SECTION("finalization (predicate on suffix)") {
        doug.addBelow(genesisBlock, blocks[1]);
        doug.notarize(blocks[1]);
        doug.addBelow(blocks[1], blocks[3]);
        doug.notarize(blocks[3]);
        doug.addBelow(blocks[3], blocks[4]);
        doug.notarize(blocks[4]);
        doug.addBelow(blocks[4], blocks[5]);
        doug.notarize(blocks[5]);

        REQUIRE(doug.getFinalizedChain().size() == 4);
    }
}
