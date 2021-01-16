#include "catch2.hpp"

#include "services/repeat_service.hpp"

TEST_CASE("Generate random payload") {
    RepeatService service(10);

    std::string payload1, payload2;
    payload1 = service.getRandomPayload();
    payload2 = service.getRandomPayload();

    REQUIRE_FALSE(payload1 == "");
    REQUIRE_FALSE(payload2 == "");
    REQUIRE_FALSE(payload1 == payload2);
}

TEST_CASE("RepeatService functionality") {
    RepeatService service(9, 3);

    REQUIRE(service.getEpoch(0) == 1);
    REQUIRE(service.getEpoch(3) == 2);

    REQUIRE(service.getLeader(1) == 0);
    REQUIRE(service.getLeader(2) == 1);
    REQUIRE(service.getLeader(12) == 2);
    REQUIRE_THROWS(service.getLeader(0));

    REQUIRE(service.isEpochStart(0));
    REQUIRE(service.isEpochStart(3));
    REQUIRE_FALSE(service.isEpochStart(1));
}

TEST_CASE("DoubleService functionality") {
    RepeatService service(9, 2, 2);

    REQUIRE(service.getLeader(1) == 0);
    REQUIRE(service.getLeader(2) == 0);
    REQUIRE(service.getLeader(3) == 1);
    REQUIRE(service.getLeader(4) == 1);
    REQUIRE_THROWS(service.getLeader(0));
}
