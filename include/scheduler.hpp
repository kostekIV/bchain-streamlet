#pragma once


class IScheduler {
public:
    virtual void start(unsigned nrRounds) = 0;
};
