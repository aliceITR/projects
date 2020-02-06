#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include "queue_array.h"
#include <vector>
#include <math.h>
#include <iomanip>

using namespace std;


//This is is the PCB Structure
struct PCB{
    int pid, 
    priority, 
    value, 
    start_time, 
    run_time, 
    cpu_time;
};

//This is the Structure that will get passed to it from the commander
struct commands{
    int pid;
    int value;
    int time;
    int rid;
    int num;
    char cmd;
    char variable;
    
   int getPid(){return pid;}
};


/*
 *TIME of the system
 *current quantum is how long the current process will run for
 *tot completed is used for the turnaround time
 *tot time is used for the turnaround time
 *Quantum is 2 
 * running state is the PID of the currently running process
 */
int TIME = 0; int currentQuantum,  QUANTUM = 2, runningState;
double totCompleted = 0, totTime = 0;
//Make a pcb table
vector<PCB> pcb_table(100);
    
    
//These are the QueueArrays for the different states
QueueArray<int> readyState (4);
QueueArray<int> blockState0 (4);
QueueArray<int> blockState1 (4);
QueueArray<int> blockState2 (4);
    

/*
 * processManager handles when time increments by one
 * Blockedprint is the print loop for all the blocked prints
 * readyprint prints all the ready processes in their respective priority categor
 * reporter forks and prints
 * print is the print function
 */    
void processManager();
void blockedPrint();
void readyPrint();
void reporter();
void print();



int main(int argc, char *argv[])
{


    //Sets up pipes and the struct recieved from the comander
    int pipes[2];
    commands judge;
    
    //this is to make the first process start as time 0
    bool start = true;
    

    
    //Intialize variables
    pipes[0] = atoi(argv[1]);
    pipes[1] = atoi(argv[2]);
    //cout << "Before Reading Pipes" << endl;
    
    
    //read from the pipe
    read(pipes[0], &judge, sizeof(commands));
    
    
    //Set up a loop to keep reading in Structures
   
    while(judge.variable != 'T')
    {
    //depending on the case do x 
       
        if(judge.variable == 'S')
        {

            //Create a new process and initialize its data
            PCB newProcess;
            newProcess.pid = judge.pid;
            newProcess.value = judge.value;
            newProcess.run_time = judge.time;
            newProcess.start_time = TIME;
            newProcess.cpu_time = 0;
            newProcess.priority = 0;
            pcb_table[newProcess.pid] = newProcess;
            if(start == true)
            {
                start = false;
                runningState = newProcess.pid;
                //Set up the first quantum here
                currentQuantum = pow(QUANTUM, newProcess.priority);
            }
           else
            {
                readyState.Enqueue(pcb_table[newProcess.pid].pid, 0);
            }
        }

        if(judge.variable == 'B')
        {
            //enqueue the running state id into the correct RID category
            //Also decrement the priority because it did not use up its quantum
            
            if(pcb_table[runningState].priority > 0)
            {
                pcb_table[runningState].priority--;
            }
            
            if( judge.rid == 0)
            {
                blockState0.Enqueue(pcb_table[runningState].pid, pcb_table[runningState].priority);
            }
           
           else if( judge.rid == 1)
            {
                blockState1.Enqueue(pcb_table[runningState].pid, pcb_table[runningState].priority);
            }
       
           else if( judge.rid == 2)
            {
                blockState2.Enqueue(pcb_table[runningState].pid, pcb_table[runningState].priority);
            }
        
            //Makes a new process the running process
            runningState = readyState.Dequeue();
            currentQuantum = pow(QUANTUM, pcb_table[runningState].priority);
           
        }
    
        if(judge.variable == 'U')
        {
            // rtn will hold the pid value of the dequeued value
            int temp;
            //dequeue the first item from te correct RID category
            
            if( judge.rid == 0)
            {
                temp = blockState0.Dequeue();
                //enqueue the pid value into the ready state
                readyState.Enqueue(pcb_table[temp].pid, pcb_table[temp].priority);
            }
        
            if( judge.rid == 1)
            {
                temp = blockState1.Dequeue();  
                //enqueue the pid value into the ready state
                readyState.Enqueue(pcb_table[temp].pid, pcb_table[temp].priority);
            }
        
            if( judge.rid == 2)
            {
                temp = blockState2.Dequeue();
                //enqueue the pid value into the ready state
                readyState.Enqueue(pcb_table[temp].pid, pcb_table[temp].priority);
            }
        
            

        
        }
    
        if(judge.variable == 'Q')
        {
           //This calls process manager to increment the systems time by one 
            processManager();
        }
    
        if (judge.variable == 'C')
        {
           //This takes a char and will do +, -, * or / depending on what was given
            if(judge.cmd == 'A')
            {
                pcb_table[runningState].value += judge.num;
            }
       
            if(judge.cmd == 'S')
            {
                pcb_table[runningState].value -= judge.num;
            }
       
            if(judge.cmd == 'M')
            {
                pcb_table[runningState].value *= judge.num;
            }
       
            if(judge.cmd == 'D')
            {
                pcb_table[runningState].value /= judge.num;
            }
            
            //call processmanager to increment time
            processManager();
        }
    
        if(judge.variable == 'P')
        {
            reporter();
        }    
    
        read(pipes[0], &judge, sizeof(commands)); 
        
    }//ends while loop
    
    //This is the T terminate statement
    //if(judge.variable == 'T')  
     double exit = totTime/totCompleted;
     //cout << totTime << endl;
     //cout << totCompleted << endl;
     cout << endl << "Turnaround Time: " << exit << endl;
    
    
    
    close(pipes[0]);
    close(pipes[1]);
    
    return 2;

}



void processManager()
{
    //increment time
    TIME++; 
    //increment the total cpuu time
    pcb_table[runningState].cpu_time++;
    //Decrement the current quantum
    currentQuantum--;
    

   
    if ( pcb_table[runningState].cpu_time ==  pcb_table[runningState].run_time)
    {
        totCompleted ++;
        totTime += (TIME - pcb_table[runningState].start_time);
        //This process is now done
        //I need to start a new process
        runningState = readyState.Dequeue();
        currentQuantum = pow(QUANTUM, pcb_table[runningState].priority);
    }
    if(currentQuantum == 0)
    {
        //increment the priority if it does not finish
        if( pcb_table[runningState].priority < 3) 
        {
             pcb_table[runningState].priority++;
        }
        
        //Put the current process back into the ready state
        readyState.Enqueue(pcb_table[runningState].pid, pcb_table[runningState].priority);
    
        //Get a process from ready state and update quantum
        runningState = readyState.Dequeue();
        currentQuantum = pow(QUANTUM, pcb_table[runningState].priority);
    }
    
}
//prints all of the blocked states
void blockedPrint()
{
    if (blockState0.QAsize() == 0)
    {
        cout << "Queue of processes Blocked for resource 0 is empty" << endl;
    }
    if (blockState0.QAsize() != 0)
    {
        cout << "Queue of processes Blocked for resource 0:" << endl;
        cout << "PID  Priority Value  Start Time  Total CPU time" << endl;
        for(int i = 0; i < 4 ; i++)
        {
            if(blockState0.Qsize(i) != 0 )
            {
                int *temp = blockState0.Qstate(i);
                for (int k = 0; k < blockState0.Qsize(i) ; k++)
                {
                    cout << setw(2)<< pcb_table[temp[k]].pid << "   " << setw(2) << pcb_table[temp[k]].priority << "       " << setw(5) << pcb_table[temp[k]].value << "     " << setw(4) << pcb_table[temp[k]].start_time << "           " << setw(3) << pcb_table[temp[k]].cpu_time << endl;
                }
            }
        }
    }
    
    if (blockState1.QAsize() == 0)
    {
        cout << "Queue of processes Blocked for resource 1 is empty" << endl;
    }
    if (blockState1.QAsize() != 0)
    {
        cout << "Queue of processes Blocked for resource 1:" << endl;
        cout << "PID  Priority Value  Start Time  Total CPU time" << endl;
        for(int i = 0; i < 4 ; i++)
        {
            if(blockState1.Qsize(i) != 0 )
            {
                int *temp = blockState1.Qstate(i);
                for (int k = 0; k < blockState1.Qsize(i) ; k++)
                {
                    cout << setw(2)<< pcb_table[temp[k]].pid << "   " << setw(2) << pcb_table[temp[k]].priority << "       " << setw(5) << pcb_table[temp[k]].value << "     " << setw(4) << pcb_table[temp[k]].start_time << "           " << setw(3) << pcb_table[temp[k]].cpu_time << endl;
                }
            }
        }
    }
    
    if (blockState2.QAsize() == 0)
    {
        cout << "Queue of processes Blocked for resource 2 is empty" << endl;
    }
     if (blockState2.QAsize() != 0)
    {
        cout << "Queue of processes Blocked for resource 2:" << endl;
        cout << "PID  Priority Value  Start Time  Total CPU time" << endl;
        for(int i = 0; i < 4 ; i++)
        {
            if(blockState2.Qsize(i) != 0 )
            {
                int *temp = blockState2.Qstate(i);
                for (int k = 0; k < blockState2.Qsize(i) ; k++)
                {
                    cout << setw(2)<< pcb_table[temp[k]].pid << "   " << setw(2) << pcb_table[temp[k]].priority << "       " << setw(5) << pcb_table[temp[k]].value << "     " << setw(4) << pcb_table[temp[k]].start_time << "           " << setw(3) << pcb_table[temp[k]].cpu_time << endl;
                }
            }
        }
    }
    
    cout << endl;
    
}


//prints the readyState
void readyPrint()
{
    if (readyState.QAsize() == 0)
    {
        cout << "Queue of processes ready is empty" << endl;
    }
    if (readyState.QAsize() != 0)
    {
        
        for(int i = 0; i < 4 ; i++)
        {
            if(readyState.Qsize(i) == 0)
            {
                cout << "Queue of processes with priority " << i << " is empty" << endl;
            }
            if(readyState.Qsize(i) != 0 )
            {
                cout << "Queue of processes with priority " << i << " :" << endl;
                cout << "PID  Priority Value  Start Time  Total CPU time" << endl;
                int *temp = readyState.Qstate(i);
                for (int k = 0; k < readyState.Qsize(i) ; k++)
                {
                    cout << setw(2)<< pcb_table[temp[k]].pid << "   " << setw(2) << pcb_table[temp[k]].priority << "       " << setw(5) << pcb_table[temp[k]].value << "     " << setw(4) << pcb_table[temp[k]].start_time << "           " << setw(3) << pcb_table[temp[k]].cpu_time << endl;
                }
            }
        }
    }   
}

void reporter()
{
    pid_t id = fork();
        if(id < 0)
    {
       cerr<<"Fork Failed";
       exit(1);
    }
    
    //child process
    if(id == 0)
    {
           print();
           exit(0);
           
    }
    else 
    {
        wait(NULL);
    }
    
    
   
    return;
}

void print()
{
    cout << "****************************************************************" << endl;
            cout << "The current system state is as follows:" << endl;
            cout << "****************************************************************" << endl << endl;
            cout << "CURRENT TIME: "<< TIME << endl << endl;
            cout << "RUNNING PROCESS: " << endl;
            cout << "PID  Priority Value  Start Time  Total CPU time" << endl;
            cout << setw(2)<< pcb_table[runningState].pid << "   " << setw(2) << pcb_table[runningState].priority << "       " << setw(5) << pcb_table[runningState].value << "     " << setw(4) << pcb_table[runningState].start_time << "           " << setw(3) << pcb_table[runningState].cpu_time << endl;
            cout << endl; 
            cout << "BLOCKED PROCESS:" << endl;
            blockedPrint();
            cout << "PROCESSES READY TO EXECUTE:" << endl;
            readyPrint();
            cout << "****************************************************************" << endl << endl;
}
    
    

