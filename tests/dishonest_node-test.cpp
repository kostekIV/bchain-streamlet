
#include "catch2.hpp"
#include "node/honest_node.hpp"
#include "node/dishonest_node.hpp"
#include "services/repeat_service.hpp"

TEST_CASE("DishonestNode behaves like a proper node") {
    Block genesis{"", 0, "GeNeZis"};

    RepeatService service{3};

    std::unordered_set<unsigned> dishonestNodes{2};

    HonestNode honest{0, 3, service, genesis};
    DishonestNode dishonest{2, 3, service, genesis, dishonestNodes};

    auto messages = honest.atTime(0);

    auto votes = dishonest.onMessageReceive(messages[2]);
    
    for(int i = 0; i < 3; i++){
        REQUIRE(votes[i].from() == 2);
        REQUIRE(votes[i].to() == i);
        REQUIRE(votes[i].content().messageType == MessageType::VOTE);
    }
}

TEST_CASE("DishonestNode likes mischief") {
    Block genesis{"", 0, "GeNeZis"};

    RepeatService service{4};

    std::unordered_set<unsigned> dishonestNodes{0};

    DishonestNode dishonest{0, 3, service, genesis, dishonestNodes};

    auto messages = dishonest.atTime(0);
    
    REQUIRE(messages.size() == 4);

    for(unsigned i = 0; i < 4; i++)
        REQUIRE(messages[i].content().messageType == MessageType::PROPOSAL);

    Block block1 = messages[0].content().block;
    Block block2 = messages[1].content().block;

    REQUIRE(block1.hash() != block2.hash());

    auto votes = dishonest.onMessageReceive(messages[2]);

    REQUIRE(votes.size() == 1);
    REQUIRE(votes[0].content().messageType == MessageType::VOTE);

    Block block3 = votes[0].content().block;
    
    votes = dishonest.onMessageReceive(messages[3]);

    REQUIRE(votes.size() == 1);
    REQUIRE(votes[0].content().messageType == MessageType::VOTE);

    Block block4 = votes[0].content().block;

    REQUIRE(block1.hash() == block3.hash());
    REQUIRE(block2.hash() == block4.hash());
}
