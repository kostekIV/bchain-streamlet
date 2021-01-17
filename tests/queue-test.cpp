#include "catch2.hpp"

#include <queue>

#include "schedulers/queue.hpp"

#define range(V) (V).begin(), (V).end()

struct DummyStruct {
    DummyStruct(int id) : id(id) {}

    const int id;

    bool operator==(const DummyStruct& rhs) const { return id == rhs.id; }

    bool operator!=(const DummyStruct& rhs) const { return !(rhs == *this); }
};

struct Comparator {
    Queue<DummyStruct> cool;
};

struct TestAction: public IQueueAction<DummyStruct> {

    TestAction(): counter(0) {}

    void onPop(DummyStruct d) override {
        if (d.id < 5) {
            counter += 2;
            queue.push({{d.id + 1}, {d.id + 2}});
        }

    };

    int counter;
    Queue<DummyStruct> queue;

};

TEST_CASE_METHOD(Comparator, "Pushing vector") {
    std::vector<DummyStruct> dummies = {{0},
                                        {1},
                                        {2},
                                        {3},
                                        {4}};

    std::queue<DummyStruct> poor;
    for (auto d: dummies)
        poor.push(d);

    cool.push(std::move(dummies));
    REQUIRE(poor == cool);
}

TEST_CASE_METHOD(Comparator, "Applying") {
    TestAction action;

    action.queue.push(std::vector<DummyStruct>{1});
    action.queue.applyToAll(action);
    /*
     * 2 3
     * 3 3 4
     * 3 4 4 5
     * 4 4 5 4 5
     * 4 5 4 5 5 6
     * 5 4 5 5 6 5 6
     * 4 5 5 6 5 6
     * 5 5 6 5 6 5 6
     * ...
     */

    REQUIRE(action.queue.empty());
    REQUIRE(action.counter == 14);
}
