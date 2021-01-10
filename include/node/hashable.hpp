#pragma once

#include <string>

using hash_t = std::string;

class Hashable {
public:
    virtual hash_t hash() const = 0;
};