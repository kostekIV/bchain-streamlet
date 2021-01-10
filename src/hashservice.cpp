#include "hashservice.hpp"

HashService::HashService(unsigned n) : n(n) {}

unsigned HashService::getLeader(unsigned epoch){
    return epoch % n;
}
