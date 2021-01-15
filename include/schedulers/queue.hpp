#pragma once

#include <functional>
#include <queue>
#include <vector>

template<typename T>
class Queue : public std::queue<T> {
public:
    using queue_action_t = std::function<std::vector<T>(T)>;

    void push(std::vector<T>&& v) { for (auto& x: v) this->emplace(x); }

    void apply(queue_action_t action) {
        while (!this->empty()) {
            auto t = this->front();
            this->pop();
            push(action(t));
        }
    }
};
