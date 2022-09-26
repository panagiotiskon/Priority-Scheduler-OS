//1115201900266
//PANAGIOTIS KONTOEIDIS


#include "semaphore.h"

    Semaphore::Semaphore(){
        this->value=1;
    }

    void Semaphore::push(Process *p){
        S_Q.push(p);
    }

    Process* Semaphore::front(){
        return S_Q.front();
    }

    void Semaphore::pop(){
        S_Q.pop();
    }

    bool Semaphore::isEmpty(){
        return S_Q.empty();
    }

    bool Semaphore::isfree(){
        return (this->value>0) ? true : false;
    }