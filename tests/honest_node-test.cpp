#include "catch2.hpp"

#include "node/honest_node.hpp"
#include "services/repeat_service.hpp"

TEST_CASE("HonestNode sends PROPOSE and VOTE") {
    Block genesis{"", 0, "GeNeZis"};

    RepeatService service{3};

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

TEST_CASE("HonestNode notarizes blocks") {
    Block genesis{"", 0, "GeNeZis"};
    Block newBlock{genesis.hash(), 1, "nasfdfa"};

    RepeatService service{10};
    HonestNode node{5, 10, service, genesis};

    Message proposal{0, 5, {MessageType::PROPOSAL, newBlock}};

    node.onMessageReceive(proposal);

    for (unsigned i = 0; i < 7; i++)
        node.onMessageReceive({i, 5, {MessageType::VOTE, newBlock}});

    REQUIRE(node.getTree().isDeepestNotarized(newBlock));
}

TEST_CASE("HonestNode does not notarize block if no more than 2/3 votes") {
    Block genesis{"", 0, "GeNeZis"};
    Block newBlock{genesis.hash(), 1, "nasfdfa"};

    RepeatService service{9};
    HonestNode node{5, 9, service, genesis};

    Message proposal{0, 5, {MessageType::PROPOSAL, newBlock}};

    node.onMessageReceive(proposal);

    for (unsigned i = 0; i < 6; i++)
        node.onMessageReceive({i, 5, {MessageType::VOTE, newBlock}});

    REQUIRE(node.getTree().isDeepestNotarized(genesis));
}

TEST_CASE("HonestNode differentiates votes") {
    Block genesis{"", 0, "GeNeZis"};
    Block newBlock{genesis.hash(), 1, "nasfdfa"};

    RepeatService service{10};
    HonestNode node{5, 10, service, genesis};

    Message proposal{0, 5, {MessageType::PROPOSAL, newBlock}};
    Message vote{2, 5, {MessageType::VOTE, newBlock}};

    node.onMessageReceive(proposal);

    for (int i = 0; i < 7; i++)
        node.onMessageReceive(vote);
    
    REQUIRE_FALSE(node.getTree().isDeepestNotarized(newBlock));
}

TEST_CASE("HonestNode ignores proposals not from leaders") {
    Block genesis{"", 0, "GeNeZis"};
    Block newBlock{genesis.hash(), 1, "nasfdfa"};

    RepeatService service{10};
    HonestNode node{5, 10, service, genesis};

    Message proposal{2, 5, {MessageType::PROPOSAL, newBlock}};

    auto messages = node.onMessageReceive(proposal);
    
    REQUIRE(messages.size() == 0);
}
