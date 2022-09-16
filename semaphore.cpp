

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

    void Semaphore::aquire(){
        this->value=0;
    }

    void Semaphore::release(){
        this->value=1;
    }

    bool Semaphore::isfree(){
        return (this->value>0) ? true : false;
    }