#include "catch2.hpp"

#include "node/honest_node.hpp"

TEST_CASE("HonestNode sends PROPOSE and VOTE") {
    Block genesis{"", 0, "GeNeZis"};
    RoundService service{3};
    HonestNode node1{0, 3, service, genesis};
    HonestNode node2{1, 3, service, genesis};

    auto messages = node1.atTime(0);

    REQUIRE(messages.size() == 3);

    for(int i = 0; i < 3; i++){
        REQUIRE(messages[i].from() == 0);
        REQUIRE(messages[i].to() == i);
        REQUIRE(messages[i].content().messageType == MessageType::PROPOSAL);
    }

    auto votes = node1.onMessageReceive(messages[0]);
    
    for(int i = 0; i < 3; i++){
        REQUIRE(votes[i].from() == 0);
        REQUIRE(votes[i].to() == i);
        REQUIRE(votes[i].content().messageType == MessageType::VOTE);
    }

    votes = node2.onMessageReceive(messages[1]);
    
    for(int i = 0; i < 3; i++){
        REQUIRE(votes[i].from() == 1);
        REQUIRE(votes[i].to() == i);
        REQUIRE(votes[i].content().messageType == MessageType::VOTE);
    }
}
