#pragma once

class RoundService{
public:
    RoundService(unsigned n);
    unsigned getLeader(unsigned epoch) const;
private:
    const unsigned n;
};
