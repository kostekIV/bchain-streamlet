#include <vector>

namespace utils {
    template <typename T>
    void insert(std::vector<T>& dest, std::vector<T>&& src) {
        for (auto&& el: src) {
            dest.push_back(el);
        }
    }
}
