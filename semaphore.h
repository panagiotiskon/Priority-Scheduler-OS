#ifndef SEMAPHORE_CPP
#define SEMAPHORE_CPP

//file contents


#include "stdio.h"
#include <stdlib.h>
#include <iostream>
#include <queue>
#include "process.h"



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
    void aquire();
    void release();
    bool isfree();
};


#endif