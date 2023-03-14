#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include <queue>
#include <fstream>


void init();
int Preemptive_Priority(int num, int s_num, int k, float lamda_ar, float lamda_b, float lamda_l);
int Priority_Inheritance(int num, int s_num, int k,float lamda_ar, float lamda_b, float lamda_l);

#endif