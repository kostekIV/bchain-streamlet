#include <functional>
#include <queue>
#include <vector>

template<typename T>
class Queue : public std::queue<T> {
public:
    void push(std::vector<T>&& v) { for (auto& x: v) this->emplace(x); }

    void tryEmptying(std::function<std::vector<T>(T)> action) {
        while (!this->empty()) {
            auto t = this->front();
            this->pop();
            push(action(t));
        }
    }
};