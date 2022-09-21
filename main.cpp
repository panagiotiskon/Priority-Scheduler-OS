//1115201900266
//PANAGIOTIS KONTOEIDIS

#include "process.h"
#include "generators.h"
#include "semaphore.h"
#include <queue>
#include <fstream>

using namespace std;

int num;
int s_num;
int k; 
float lamda_a;
float lamda_b;
float lamda_l;
int random_s;

Process* temp; 
Process* s_temp;

int temp_p=10;
int temp_pid;

int terminated_processes=0;
int sem_index=-1;

int priority_table[7] = {0,0,0,0,0,0,0};
int waiting_time[7]={0,0,0,0,0,0,0};
int blocking_time[7]={0,0,0,0,0,0,0};
int processes_by_priority[7]={0,0,0,0,0,0,0};

void init(){
    for(int i=0; i<7; i++){
        priority_table[i]=0;
        waiting_time[i]=0;
        blocking_time[i]=0;
        processes_by_priority[i]=0;
    }
}

int Preemptive_Priority(int num, int s_num, int k, float lamda_ar, float lamda_b, float lamda_l){

    ofstream MyFile("Preemptive_Priority.txt");
    priority_queue<Process*, vector<Process*>, ComparePriority> Q;

    int* priority= uniform_generator(num);
    int* arrival_t = exponential_generator(lamda_ar, num);
    int* burst_t = exponential_generator(lamda_b, num);
    int* lifetime_t = exponential_generator(lamda_l, num);

    Process *p[num];
    Semaphore *s[s_num];

    for(int i=0; i<num; i++){      //create processes 
        p[i] = new Process(i, arrival_t[i], burst_t[i], lifetime_t[i], priority[i]);
        processes_by_priority[p[i]->priority-1]++;
    }

    for(int i=0;i<s_num;i++){       //create semaphores
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

    MyFile<<" The Processes are: \n\n";

    for(int i=0;i<num;i++){
        MyFile<<" Pid "<< p[i]->pid<<" arrival "<<p[i]->arrival_time<<" burst "<<p[i]->burst_time<<" priority "<<p[i]->get_priority()<<" service "<<p[i]->lifetime<<" state "<<p[i]->state<<" \n";
    }
    cout<<endl;
    
    for(int time=0; time<3000; time+=1){

        terminated_processes=0;

        for(int i=0; i<num; i++){    
            p[i]->running=-1;
        }

        for(int i=0; i<num; i++){                        //when time==arrival time push to queue
            if(p[i]->get_arr_time()==time && (p[i]->lifetime>0 )){        
                p[i]->state = "READY";
                Q.push(p[i]);
            }
            else if(p[i]->lifetime<=0){
                p[i]->state = "TERMINATED";
                terminated_processes++;
            }            
        }


        if(terminated_processes==num){          //when all processes are terminated break
            break;
        }   

        MyFile<<"\nTime is: "<<time<<"\n";

        for(int i=0; i<s_num; i++){                  //check for the process with maximum priority locking a semaphore
            if(s[i]->value==0){
                if(s[i]->curr_process->get_priority()<temp_p){
                    s_temp = s[i]->curr_process;
                    temp_p = s_temp ->get_priority();
                    sem_index=i;
                }
            }
        }

        if(!Q.empty()){

            if(Q.top()->get_priority()<temp_p && (Q.top()->priority)>0){     //check if priority in ready queue is greater than priority of proc in sems
                
                temp = Q.top();
                temp->running=1;

                if(propability(k) && temp->lifetime>1 ){                        //process tries to execute down()
                    
                    random_s = random_generator(s_num);
                    Q.pop();
                    temp->lifetime-=1;

                    if((s[random_s]->value)==1){   
                        temp ->state ="RUNNING";
                        s[random_s]->curr_process = temp;
                        s[random_s]->value=0;
                        for(int i=0; i<s_num; i++){
                            if(i!=random_s && !s[i]->isEmpty())
                                s[i]->curr_process->state="WAITING";
                        }
                    }
                    else{
                        temp->state = "BLOCKED";
                        temp->blocked = 1;
                        s[random_s]->push(temp);
                    }

                }
                else if(temp->lifetime>0){                                      //if it fails to execute down()
                    temp->lifetime-=1;             
                    temp->state="RUNNING";
                    for(int i=0; i<s_num; i++){
                            if(!s[i]->isEmpty())
                                s[i]->curr_process->state="WAITING";}
                }

                else if(temp->lifetime<=0){
                    temp->state= "TERMINATED";
                    temp->running=-1;
                    Q.pop();
                }
            }

            else if(temp_p!=10 && s_temp->burst_time!=0){                                
                s_temp->burst_time-=1;
                s_temp->running=1;

                    for(int i=0; i<s_num; i++){
                        if(i!=sem_index && !s[i]->isEmpty())
                            s[i]->curr_process->state="WAITING";
                    }
            }
            else if(temp_p!=10 && s_temp->burst_time == 0){     //if process finises up() then push it back to the ready queue
                s_temp->state="READY";
                s_temp->burst_time= s_temp->remaining_time;

                if(s_temp->lifetime>0){
                    s_temp->lifetime--;
                    s_temp->running=1;
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
                    s[sem_index]->curr_process->blocked=-1;

                    for(int i=0; i<s_num; i++){
                        if(i!=sem_index && !s[i]->isEmpty())
                            s[i]->curr_process->state="WAITING";
                    }
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

                if((s_temp->lifetime-1)>=0) 
                    s_temp->lifetime--;
                    s_temp->running=1;

                if(s[sem_index]->isEmpty()){                    //if sem queue is empty
                    s[sem_index]->value=1;
                    s[sem_index]->curr_process = NULL;
                }
                else{
                    s[sem_index]->S_Q.front()->state = "RUNNING";
                    s[sem_index]->curr_process = s[sem_index]->front();
                    s[sem_index]->S_Q.pop();
                    s[sem_index]->curr_process->blocked=-1;
                    for(int i=0; i<s_num; i++){
                        if(i!=sem_index && !s[i]->isEmpty())
                            s[i]->curr_process->state="WAITING";
                    }
                }
            }
            else if(s_temp->burst_time>0){
                s_temp->state="RUNNING";
                s_temp->burst_time-=1;
                s_temp->running=1;

                for(int i=0; i<s_num; i++){
                    if(i!=sem_index && !s[i]->isEmpty())
                        s[i]->curr_process->state="WAITING";
                }
            }
            else if(s_temp->lifetime<=0){
                s[sem_index]->value=1;
            }
        }

        else if(Q.empty() && temp_p==10){       //if queue is empty and no proc in temps
            continue;               
        }

        temp_p=10;

        for(int i=0; i<num; i++){                                           //select waiting and blocking time from each process

            if(p[i]->arrival_time<=time && p[i]->running==-1 && p[i]->blocked==-1 && p[i]->state!="TERMINATED"){
                waiting_time[p[i]->priority-1]++;
            }
            else if(p[i]->arrival_time<=time && p[i]->running==-1 && p[i]->blocked==1 && p[i]->state!="TERMINATED"){
                blocking_time[p[i]->priority-1]++;
            }
        }

        for(int i=0;i<num;i++){
            MyFile<<" Pid "<< p[i]->pid<<" arrival "<<p[i]->arrival_time<<" burst "<<p[i]->burst_time<<" priority "<<p[i]->get_priority()<<" service "<<p[i]->lifetime<<" state "<<p[i]->state<<" \n";
        }

        MyFile<<"-------------------------------------------------------\n";      
    }

    for(int i=0;i<num;i++){
            MyFile<<" Pid "<< p[i]->pid<<" arrival "<<p[i]->arrival_time<<" burst "<<p[i]->burst_time<<" priority "<<p[i]->get_priority()<<" service "<<p[i]->lifetime<<" state "<<p[i]->state<<" \n";
    }

    MyFile<<"-------------------------------------------------------\n";

    MyFile<<"\nAverage Waiting Time by priority is:\n\n";
    for(int i=0; i<7; i++){
        float res;
        if(waiting_time[i]==0)
            res=0;
        else
             res= (float) waiting_time[i] / processes_by_priority[i];
        MyFile<<"Processes with Priority "<<i+1<<" : "<<res<<"\n";
    }

    MyFile<<"-------------------------------------------------------\n";

    MyFile<<"\nAverage Blocking Time by priority is:\n\n";
    for(int i=0; i<7; i++){
        float res;
        if(waiting_time[i]==0)
            res=0;
        else
             res= (float) blocking_time[i] / processes_by_priority[i];
        MyFile<<"Processes with Priority "<<i+1<<" : "<<res<<"\n";
    }

    MyFile.close();
    return 0;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int Priority_Inheritance(int num, int s_num, int k,float lamda_ar, float lamda_b, float lamda_l){

    ofstream MyFile("Priority_Inheritance.txt");
    priority_queue<Process*, vector<Process*>, ComparePriority> Q;

    int* priority= uniform_generator(num);
    int* arrival_t = exponential_generator(lamda_ar, num);
    int* burst_t = exponential_generator(lamda_b, num);
    int* lifetime_t = exponential_generator(lamda_l, num);

    Process *p[num];
    Semaphore *s[s_num];

    for(int i=0; i<num; i++){      //create processes 
        p[i] = new Process(i, arrival_t[i], burst_t[i], lifetime_t[i], priority[i]);
        processes_by_priority[p[i]->priority-1]++;
    }

    for(int i=0;i<s_num;i++){       //create semaphores
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

    MyFile<<" The Processes are: \n\n";

    for(int i=0;i<num;i++){
        MyFile<<" Pid "<< p[i]->pid<<" arrival "<<p[i]->arrival_time<<" burst "<<p[i]->burst_time<<" priority "<<p[i]->get_priority()<<" service "<<p[i]->lifetime<<" state "<<p[i]->state<<" \n";
    }
    cout<<endl;
    
    for(int time=0; time<3000; time+=1){

        terminated_processes=0;

        for(int i=0; i<num; i++){    
            p[i]->running=-1;
        }

        for(int i=0; i<num; i++){                        //when time==arrival time push to queue
            if(p[i]->get_arr_time()==time && (p[i]->lifetime>0 )){        
                p[i]->state = "READY";
                Q.push(p[i]);
            }
            else if(p[i]->lifetime<=0){
                p[i]->state = "TERMINATED";
                terminated_processes++;
            }            
        }

        if(terminated_processes==num){          //when all processes are terminated break
            break;
        }   

        MyFile<<"\nTime is: "<<time<<"\n";

        for(int i=0; i<s_num; i++){                  //check for the process with maximum priority locking a semaphore
            if(s[i]->value==0){
                if(s[i]->curr_process->get_priority()<temp_p){
                    s_temp = s[i]->curr_process;
                    temp_p = s_temp ->get_priority();
                    sem_index=i;
                }
            }
        }

        if(!Q.empty()){

            if(Q.top()->get_priority()<temp_p && (Q.top()->priority)>0){     //check if priority in ready queue is greater than priority of proc in sems
                
                temp = Q.top();
                temp->running=1;

                if(propability(k) && temp->lifetime>1 ){                        //process tries to execute down()
                    
                    random_s = random_generator(s_num);
                    Q.pop();
                    temp->lifetime-=1;

                    if((s[random_s]->value)==1){   
                        temp ->state ="RUNNING";
                        s[random_s]->curr_process = temp;
                        s[random_s]->value=0;

                        for(int i=0; i<s_num; i++){
                            if(i!=random_s && !s[i]->isEmpty())
                                s[i]->curr_process->state="WAITING";
                        }
                    }
                    else{
                        temp->state = "BLOCKED";
                        temp->blocked = 1;
                        s[random_s]->push(temp);
                        if(temp->priority < s[random_s]->curr_process->priority  ){       //priority inheritance
                            s[random_s]->curr_process->priority = temp->priority;
                        }
                    }

                } 
                else if(temp->lifetime>0){                                      //if it fails to execute down()
                    temp->lifetime-=1;             
                    temp->state="RUNNING";
                    for(int i=0; i<s_num; i++){
                            if(!s[i]->isEmpty())
                                s[i]->curr_process->state="WAITING";}
                }

                else if(temp->lifetime<=0){
                    temp->state= "TERMINATED";
                    temp->running=-1;
                    Q.pop();
                }
            }

            else if(temp_p!=10 && s_temp->burst_time!=0){                                
                s_temp->burst_time-=1;
                s_temp->running=1;

                    for(int i=0; i<s_num; i++){
                        if(i!=sem_index && !s[i]->isEmpty())
                            s[i]->curr_process->state="WAITING";
                    }
            }
            else if(temp_p!=10 && s_temp->burst_time == 0){     //if process finises up() then push it back to the ready queue
                s_temp->state="READY";
                s_temp->burst_time= s_temp->remaining_time;

                if(s_temp->lifetime>0){
                    s_temp->lifetime--;
                    s_temp->running=1;
                    s_temp->priority = s_temp->t_priority;      //restore priority
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
                    s[sem_index]->curr_process->blocked=-1;

                    for(int i=0; i<s_num; i++){
                        if(i!=sem_index && !s[i]->isEmpty())
                            s[i]->curr_process->state="WAITING";
                    }
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
                s_temp->priority = s_temp->t_priority;
                Q.push(s_temp);

                if((s_temp->lifetime-1)>=0) 
                    s_temp->lifetime--;
                    s_temp->running=1;

                if(s[sem_index]->isEmpty()){                    //if sem queue is empty
                    s[sem_index]->value=1;
                    s[sem_index]->curr_process = NULL;
                }
                else{
                    s[sem_index]->S_Q.front()->state = "RUNNING";
                    s[sem_index]->curr_process = s[sem_index]->front();
                    s[sem_index]->S_Q.pop();
                    s[sem_index]->curr_process->blocked=-1;
                    for(int i=0; i<s_num; i++){
                        if(i!=sem_index && !s[i]->isEmpty())
                            s[i]->curr_process->state="WAITING";
                    }
                }
            }
            else if(s_temp->burst_time>0){
                s_temp->state="RUNNING";
                s_temp->burst_time-=1;
                s_temp->running=1;

                for(int i=0; i<s_num; i++){
                    if(i!=sem_index && !s[i]->isEmpty())
                        s[i]->curr_process->state="WAITING";
                }
            }
            else if(s_temp->lifetime<=0){
                s[sem_index]->value=1;
            }
        }

        else if(Q.empty() && temp_p==10){       //if queue is empty and no proc in temps
            continue;               
        }

        temp_p=10;

        for(int i=0; i<num; i++){                                           //select waiting and blocking time from each process

            if(p[i]->arrival_time<=time && p[i]->running==-1 && p[i]->blocked==-1 && p[i]->state!="TERMINATED"){
                waiting_time[p[i]->priority-1]++;
            }
            else if(p[i]->arrival_time<=time && p[i]->running==-1 && p[i]->blocked==1 && p[i]->state!="TERMINATED"){
                blocking_time[p[i]->priority-1]++;
            }
        }

        for(int i=0;i<num;i++){
            MyFile<<" Pid "<< p[i]->pid<<" arrival "<<p[i]->arrival_time<<" burst "<<p[i]->burst_time<<" priority "<<p[i]->get_priority()<<" service "<<p[i]->lifetime<<" state "<<p[i]->state<<" \n";
        }

        MyFile<<"-------------------------------------------------------\n";      
    }
    for(int i=0;i<num;i++){
            MyFile<<" Pid "<< p[i]->pid<<" arrival "<<p[i]->arrival_time<<" burst "<<p[i]->burst_time<<" priority "<<p[i]->get_priority()<<" service "<<p[i]->lifetime<<" state "<<p[i]->state<<" \n";
    }

    MyFile<<"-------------------------------------------------------\n";

    MyFile<<"\nAverage Waiting Time by priority is:\n\n";
    for(int i=0; i<7; i++){
        float res;
        if(waiting_time[i]==0)
            res=0;
        else
             res= (float) waiting_time[i] / processes_by_priority[i];
        MyFile<<"Processes with Priority "<<i+1<<" : "<<res<<"\n";
    }

    MyFile<<"-------------------------------------------------------\n";

    MyFile<<"\nAverage Blocking Time by priority is:\n\n";
    for(int i=0; i<7; i++){
        float res;
        if(waiting_time[i]==0)
            res=0;
        else
             res= (float) blocking_time[i] / processes_by_priority[i];
        MyFile<<"Processes with Priority "<<i+1<<" : "<<res<<"\n";
    }

    MyFile.close();
    return 0;
}

int main(){


    cout<<"how many processes? "<<endl;
    cin>>num;
    
    cout<<"how many semaphores?"<<endl;
    cin>>s_num;

    cout<<"enter a value between 0-100 for the propability"<<endl;
    cin>>k;

    cout<<"enter lamda for arrival of processes"<<endl;
    cin>>lamda_a;
    
    cout<<"enter lamda for lifetime of processes"<<endl;
    cin>>lamda_l;    
    
    cout<<"enter lamda for critical section of processes"<<endl;
    cin>>lamda_b;

    Preemptive_Priority(num, s_num, k, lamda_a, lamda_b, lamda_l);
    init();
    Priority_Inheritance(num, s_num, k, lamda_a, lamda_b, lamda_l);

    return 0;

}
