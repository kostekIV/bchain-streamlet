#include "catch2.hpp"

#include "state/block.hpp"
#include "state/tree/tree.hpp"
#include "state/tree/tree_union.hpp"
#include "state/tree/tree_renderer.hpp"


namespace {
    const Block& castFromHashable(const std::reference_wrapper<const Hashable>& ref) {
        return *dynamic_cast<const Block *>(&ref.get());
    }

    bool finalizationPredicate(const std::vector<std::reference_wrapper<const Hashable>>& blocks) {
        return true;
    }

    struct TreeTestFixture {

        void initialiseTree1() {
            int j = 0;
            tree1.addBelow(genesisBlock, blocks1[0]);
            for (int i = 1; i < 5; i++) {
                tree1.addBelow(blocks1[i - 1], blocks1[i]);
            }
        }

        void initialiseTree2() {
            int j = 0;
            tree2.addBelow(genesisBlock, blocks2[0]);
            for (int i = 1; i < 5; i++) {
                tree2.addBelow(blocks2[i - 1], blocks2[i]);
            }
        }
        void initialiseTree3() {
            int j = 0;
            for (int i = 1; i < 5; i++) {
                tree3.addBelow(blocks3[i - 1], blocks3[i]);
            }
        }

        void notirizeAll() {
            for (auto& b: blocks1) {
                tree1.notarize(b);
            }
            for (auto& b: blocks2) {
                tree2.notarize(b);
            }
        }

        Block genesisBlock{"", 0, "payload-0"};
        std::vector<Block> blocks1 = {
            {"0", 1, "sdlkfjsdf"},
            {"1", 2, "sdlkfjsdf"},
            {"2", 3, "sdlkfjsdf"},
            {"3", 4, "sdlkfjsdf"},
            {"4", 5, "sdlkfjsdf"}
        };
        std::vector<Block> blocks2 = {
            {"0", 11, "sdlkfjsdf"},
            {"11", 12, "sdlkfjsdf"},
            {"12", 13, "sdlkfjsdf"},
            {"13", 14, "sdlkfjsdf"},
            {"14", 15, "sdlkfjsdf"}
        };
        std::vector<Block> blocks3 = {
            {"16", 21, "sdlkfjsdf"},
            {"21", 33, "sdlkfjsdf"},
            {"33", 35, "sdlkfjsdf"},
            {"35", 45, "sdlkfjsdf"},
            {"45", 46, "sdlkfjsdf"}
        };
        Tree tree1{genesisBlock, finalizationPredicate};
        Tree tree2{genesisBlock, finalizationPredicate};
        Tree tree3{blocks3[0], finalizationPredicate};
    };
}

TEST_CASE_METHOD(TreeTestFixture, "Constructor from tree") {
    initialiseTree1();

    TreeUnion trees(tree1);

    REQUIRE(true);
}


TEST_CASE_METHOD(TreeTestFixture, "Constructor from tree same as insert tree") {
    initialiseTree1();

    TreeUnion trees1(tree1);
    TreeUnion trees2;
    trees2.add(tree1);

    REQUIRE(trees1 == trees2);
}

TEST_CASE_METHOD(TreeTestFixture, "Adding multiple times same tree") {
    initialiseTree1();

    TreeUnion trees1(tree1);
    TreeUnion trees2;

    trees2.add(tree1)
        .add(tree1)
        .add(tree1)
        .add(tree1);

    REQUIRE(trees1 == trees2);
}

TEST_CASE_METHOD(TreeTestFixture, "Adding tree1 then tree2 same as adding tree2 and then tree1") {
    initialiseTree1();
    initialiseTree2();

    TreeUnion trees1;
    trees1
        .add(tree1)
        .add(tree2);
    TreeUnion trees2;
    trees2
        .add(tree2)
        .add(tree1);

    REQUIRE(trees1 == trees2);
}

TEST_CASE_METHOD(TreeTestFixture, "Adding 2 disjoint trees result in invalidTree") {
    initialiseTree2();
    initialiseTree3();

    TreeUnion trees1;
    trees1
        .add(tree2)
        .add(tree3);

    REQUIRE(trees1.isValidTree() == false);
}

TEST_CASE_METHOD(TreeTestFixture, "Adding 2 trees with the same root result in validTree") {
    initialiseTree1();
    initialiseTree2();

    TreeUnion trees1;
    trees1
        .add(tree1)
        .add(tree2);

    REQUIRE(trees1.isValidTree());
}

TEST_CASE_METHOD(TreeTestFixture, "Adding 2 disjoint trees result in not connected Tree") {
    initialiseTree2();
    initialiseTree3();

    TreeUnion trees1;
    trees1
        .add(tree2)
        .add(tree3);

    REQUIRE(trees1.isConnected() == false);
}

TEST_CASE_METHOD(TreeTestFixture, "Adding 2 trees with the same root result in connected Tree") {
    initialiseTree1();
    initialiseTree2();

    TreeUnion trees1;
    trees1
        .add(tree1)
        .add(tree2);

    REQUIRE(trees1.isConnected());
}


TEST_CASE_METHOD(TreeTestFixture, "Finalised chain is proper") {
    initialiseTree1();
    initialiseTree2();
    initialiseTree3();
    notirizeAll();

    TreeUnion trees1;
    trees1
        .add(tree2);

    trees1.print();

    REQUIRE(trees1.isFinalizedCorrectly());
}

TEST_CASE_METHOD(TreeTestFixture, "Adding 2 trees with same root results in bad finalised chain") {
    initialiseTree1();
    initialiseTree2();
    initialiseTree3();
    notirizeAll();

    TreeUnion trees1;
    trees1
        .add(tree1)
        .add(tree2);

    REQUIRE(trees1.isFinalizedCorrectly() == false);
}

TEST_CASE_METHOD(TreeTestFixture, "After adding 2 trees returned mapping contains all nodes") {
    initialiseTree1();
    initialiseTree2();

    TreeUnion trees1;
    trees1
        .add(tree1)
        .add(tree2);

    auto ret = trees1.asHashVertexMapping();

    for (auto h: {"0", "1", "2", "3", "4", "5", "11", "12", "13", "14", "15"}) {
        REQUIRE(ret.find(h) != ret.end());
    }
}

TEST_CASE_METHOD(TreeTestFixture, "After adding 2 trees returned mapping have correct statuses") {
    initialiseTree1();
    initialiseTree2();
    initialiseTree3();
    notirizeAll();

    TreeUnion trees1;
    trees1
        .add(tree1)
        .add(tree2);

    auto ret = trees1.asHashVertexMapping();

    for (auto h: {"0", "1", "2", "3", "4", "11", "12", "13", "14"}) {
        REQUIRE(ret.at(h).getStatus() == Status::FINALIZED);
    }
    for (auto h: {"5", "15"}) {
        REQUIRE(ret.at(h).getStatus() == Status::NOTARIZED);
    }
}
