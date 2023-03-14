//1115201900266
//PANAGIOTIS KONTOEIDIS

#ifndef PROCESS_CPP
#define PROCESS_CPP

#include <iostream>
#include <stdlib.h>

using namespace std;

class Process{
    public:
        int pid;
        int priority;
        int arrival_time;
        int burst_time;
        int remaining_time;
        int lifetime;
        string state;
        int running; 
        int blocked;
        int t_priority;

        Process();
        Process(int, int, int, int, int);
        int get_priority();
        int get_arr_time();
        int get_burst_time();
        int get_rem_time();
        void print_all();
};


#endif