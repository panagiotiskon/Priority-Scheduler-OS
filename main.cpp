#include "process.h"
#include "generators.h"
#include "semaphore.h"
#include <queue>

using namespace std;

int num;
int s_num;
int k; 
int lamda;

int random_s;

Process* temp; 
Process* s_temp;

int temp_p=10;
int temp_pid;

int terminated_processes=0;
int sem_index=-1;
int main(){

    cout<<"how many processes? "<<endl;
    cin>>num;
    
    cout<<"how many semaphores?"<<endl;
    cin>>s_num;

    // cout<<""<<endl;


    priority_queue<Process*, vector<Process*>, ComparePriority> Q;

    int* priority= uniform_generator(num);
    int* arrival_t = exponential_generator(1, num);
    int* burst_t = exponential_generator(1, num);
    int* lifetime_t = exponential_generator(1, num);

    Process *p[num];
    Semaphore *s[s_num];

    for(int i=0; i<num; i++){      //create processes 
        p[i] = new Process(i, arrival_t[i], burst_t[i], lifetime_t[i], priority[i]);
    }

    for(int i=0;i<s_num;i++){
        s[i] = new Semaphore();
    }
    
    for(int i=0; i<num-1; i++){   //sort processes by their arrival time
        for(int j=i+1; j<num; j++){
            if(p[j]->get_arr_time()<p[i]->get_arr_time()){
                Process* t = p[j];
                p[j] = p[i];
                p[i] = t;
            }   
        }
    }

    for(int i=0; i<num; i++){
        p[i]->print_all();
    }

    cout<<endl;

        for(int time=0; time<3000; time+=1){

            terminated_processes=0;

            for(int i=0; i<num; i++){                        //if process arrives push to the priority queue
                if(p[i]->get_arr_time()==time && (p[i]->lifetime >0 )){        
                    p[i]->state = "READY";
                    Q.push(p[i]);
                }
                else if(p[i]->lifetime<=0){
                    p[i]->state = "TERMINATED";
                    terminated_processes++;
                }            
            }

            
            cout<<time<<endl;
            for(int i=0; i<num; i++){
                p[i]->print_all();
            }

            if(terminated_processes==num){
                break;
            }   
        
            for(int i=0; i<s_num; i++){                  //check for the process with maximum priority in semaphores
                if(!(s[i]->isfree())){
                    if(s[i]->curr_process->get_priority()<temp_p){
                        s_temp = s[i]->curr_process;
                        temp_p = temp ->get_priority();
                        sem_index=i;
                    }
                }
            }

            if(!Q.empty()){
                if(Q.top()->get_priority()<temp_p && (Q.top()->priority)>0){     //check if priority in ready queue is greater than priority of proc in sems
                    
                    temp = Q.top();

                    if(propability(90) && temp->lifetime!=0){                        //process tries to execute down()
                        
                        random_s = random_generator(s_num);
                        Q.pop();

                        temp->lifetime-=1;
                        if((s[random_s]->value)==1){
                            temp ->state ="RUNNING";
                            s[random_s]->curr_process = temp;
                            s[random_s]->value=0;
                        }
                        else{
                            temp->state = "BLOCKED";
                            s[random_s]->push(temp);
                        }

                    }
                    else if(temp->lifetime>0){                                      //if it fails to execute down()
                        temp->lifetime-=1;             
                    }

                    else if(temp->lifetime<=0){
                        temp->state= "TERMINATED";
                        Q.pop();
                    }

                }
                else if(temp_p!=10 && s_temp->burst_time!=0){                                
                    s_temp->burst_time-=1;
                }

                else if(temp_p!=10 && s_temp->burst_time == 0){     //if process finises up() then push it back to the ready queue
                    s_temp->state="READY";
                    s_temp->burst_time= s_temp->remaining_time;

                    if(s_temp->lifetime>0){
                        Q.push(s_temp);}
                    
                    else if(s_temp->lifetime<=0){
                        s_temp->state="TERMINATED";
                    }

                    if(s[sem_index]->isEmpty()){                    //if sem queue is empty
                        s[sem_index]->value=1;
                        s[sem_index]->curr_process = NULL;
                    }
                    else{
                        s[sem_index]->S_Q.front()->state = "RUNNING";
                        s[sem_index]->curr_process = s[sem_index]->S_Q.front();
                        s[sem_index]->S_Q.pop();
                    }
                }

                else if(Q.top()->lifetime==0){
                    Q.pop();
                }



            }
            else if(Q.empty() && temp_p!=10){         //if queue is empty but sems have processes

                if(s_temp->burst_time==0){
                    s_temp->state="READY";
                    s_temp->burst_time= s_temp->remaining_time;
                    Q.push(s_temp);
                    s_temp->lifetime--;

                    if(s[sem_index]->isEmpty()){                    //if sem queue is empty
                        s[sem_index]->value=1;
                        s[sem_index]->curr_process = NULL;
                    }
                    else{
                        s[sem_index]->S_Q.front()->state = "RUNNING";
                        s[sem_index]->curr_process = s[sem_index]->front();
                        s[sem_index]->S_Q.pop();
                    }
                }
                else
                    s_temp->burst_time-=1;

            }

            else if(Q.empty() && temp_p==10){               //if queue is empty but there is a proc in sems
                continue;
            }

            temp_p=10;
        }

    cout<<endl;
    return 0;

}
