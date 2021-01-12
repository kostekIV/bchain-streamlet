#pragma once

class HashService{
public:
    HashService(unsigned n);
    unsigned getLeader(unsigned epoch) const;
private:
    const unsigned n;
};
