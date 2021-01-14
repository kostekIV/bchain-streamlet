#include <functional>
#include <queue>
#include <vector>

template<typename T>
class Queue {
public:
    void push(T&& t) { Q.push(t); }

    void push(std::vector<T>&& v) { for (auto& x: v) Q.emplace(x); }

    void tryEmptying(std::function<T(T)> action) {
        tryEmptying([&](T t) -> std::vector<T> { return {action(t)}; });
    }

    void tryEmptying(std::function<std::vector<T>(T)> action) {
        while (!Q.empty()) {
            auto t = Q.front();
            Q.pop();
            push(action(t));
        }
    }

private:
    std::queue<T> Q;
};