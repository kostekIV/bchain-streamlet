#pragma once

class HashService{
    unsigned n;
public:
    HashService(unsigned n);

    unsigned getLeader(unsigned epoch);
};
