//1115201900266
//PANAGIOTIS KONTOEIDIS

#include "process.hpp"

using namespace std;

Process::Process(){}

Process::Process(int Pid, int ar_t, int b_t, int l_t, int pr){
    this->pid=Pid;
    this->priority=pr;
    this->arrival_time=ar_t;
    this->burst_time=b_t;
    this->remaining_time=burst_time;
    this->lifetime = l_t;
    this->running=-1;
    this->blocked=-1;
    this->t_priority=pr;
}

int Process::get_priority(){
    return this->priority;
}

int Process::get_arr_time(){
    return this->arrival_time;
}

int Process::get_burst_time(){
    return this->burst_time;
}

int Process::get_rem_time(){
    return this->remaining_time;
}


void Process::print_all(){
    cout<<"Process pid:"<<this->pid<<" arrival time: "<<this->arrival_time<<" burst time: "<<this->burst_time;
    cout<<" priority: "<<this->priority<<" lifetime "<<this->lifetime<<" state "<<this->state<<endl;
}


