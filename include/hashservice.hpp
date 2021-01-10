#pragma once

class HashService{
public:
    HashService(unsigned n);
    unsigned getLeader(unsigned epoch);
private:
    unsigned n;
};
