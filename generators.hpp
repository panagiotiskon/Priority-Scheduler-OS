//1115201900266
//PANAGIOTIS KONTOEIDIS

#ifndef GENERATORS_HPP
#define GENERATORS_HPP

#include <vector>
#include <cstdlib>
#include <chrono>
#include <random>



int* exponential_generator(float median, int num);   //use exponential distribution
int* uniform_generator(int num);   //use uniform distribution
int random_generator(int max);       //returns random number from[0, max-1]
bool propability(int k);

#endif