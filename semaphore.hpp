//1115201900266
//PANAGIOTIS KONTOEIDIS

#ifndef SEMAPHORE_CPP
#define SEMAPHORE_CPP


#include "stdio.h"
#include <stdlib.h>
#include <iostream>
#include <queue>
#include "process.hpp"


using namespace std;

class Semaphore {
    public:

    int value;
    Process* curr_process;
    queue <Process*> S_Q;

    Semaphore();   
     
    void push(Process *p);
    Process* front();
    void pop();
    bool isEmpty();
    bool isfree();
};


#endif