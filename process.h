
#ifndef PROCESS_CPP
#define PROCESS_CPP


#include "stdio.h"
#include <stdlib.h>
#include <iostream>

using namespace std;


class Process{
    public:
        unsigned int pid;
        unsigned int priority;
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
        void set_state(string s);
        int get_priority();
        int get_arr_time();
        int get_burst_time();
        int get_rem_time();
        void print_all();
};


#endif