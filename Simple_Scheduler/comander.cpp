#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

struct commands{
    int pid;
    int value;
    int time;
    int rid;
    int num;
    char cmd;
    char variable;
};


int main()//int argc, char** argv if i need these
{
    int status, pipes[2];
    char READ[10], WRITE[10], chr;
    
    
    //Try to create the pipe
    if(pipe(pipes) == -1)
        {
           cerr<<"Pipe Failed"<<endl;
           exit(1);
        }

     sprintf(READ, "%d", pipes[0]);
     sprintf(WRITE, "%d", pipes[1]);
    
    //Try to fork the pipe
    
    pid_t id = fork();   
    
    //See if the fork failed and if so exit
    
    if(id < 0)
    {
       cerr<<"Fork Failed";
       exit(1);
    }
    
    
    //if equal to 0 open the file
    
    if(id == 0)
    {
            //use the execl function to open the file
        execl("processManager", "processManager", READ, WRITE, NULL);
    }
    
    //create a commands structure to judge what is passed through
    
    commands judge;
    
    //go through the inputs in a while loop and pass them to the pm
    //most of te checks have nothing, but something can be implemened later if i revist this code
    while(cin >> judge.variable)
    {
            
            if(judge.variable == 'S')
            {
                
                cin >> judge.pid;
                cin >> judge.value;
                cin >> judge.time;
            }
            
            else if(judge.variable == 'B')
            {
                
                cin >> judge.rid;
            }
            
            else if(judge.variable == 'U')
            {
                
                cin >> judge.rid;
            }
           
           else if(judge.variable == 'Q')
            {
                
                
            }
           
           else if (judge.variable == 'C')
            {
                
                cin >> judge.cmd;
                cin >> judge.num;   
            }
            
            else if(judge.variable == 'P')
            {
                
            }
            
            else if(judge.variable == 'T')
            {
                
            }
            
            //pass the command to the pm
           
            write(pipes[1], (commands *)&judge, sizeof(commands));
           
            sleep(2);
    }
    
        

    //Close Pipes
    close(pipes[0]);
    close(pipes[1]);
    
    //Gets the status of the children
    wait(&status);
    exit(0);

}

