#pragma once

#include <functional>
#include <queue>
#include <vector>


template<typename T>
class IQueueAction {
    public:
        virtual void onPop(T elem) = 0;
        virtual ~IQueueAction() = 0;
};

template<typename T>
inline IQueueAction<T>::~IQueueAction() {};

template<typename T>
class Queue : public std::queue<T> {
public:
    void push(std::vector<T>&& v) { for (auto& x: v) this->emplace(x); }

    void applyToAll(IQueueAction<T>& action) {
        while (!this->empty()) {
            auto t = this->front();
            this->pop();
            action.onPop(t);
        }
    }
};
