//1115201900266
//PANAGIOTIS KONTOEIDIS

#ifndef GENERATORS_HPP
#define GENERATORS_HPP

#include "process.hpp"

struct ComparePriority
{
    bool operator()(Process *p1, Process *p2)
    {
        return p2->get_priority() < p1->get_priority(); // return "true" if "p1" is ordered before "p2"
    }
};

int* exponential_generator(float median, int num);   //use exponential distribution
int* uniform_generator(int num);   //use uniform distribution
int random_generator(int max);       //returns random number from[0, max-1]
bool propability(int k);

#endif