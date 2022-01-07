#include <unistd.h>
#include <iostream>
using namespace std;

void perticularCodeAfterProcess2Terminated(){
    //running perticular this code after termination of p2
    cout << "This is perticular code to be executed only when p2 is terminated\n";
}

int main(int argc, char* argv[])
{
    int pid2, pid3;
    int status;
    pid2 = fork();
    if (pid2 > 0) {
        pid3 = fork();
        if (pid3 > 0) {
            //parent
            int firstReturnedProcessPID = wait(&status);
            
            if (firstReturnedProcessPID == pid2) {
                cout << "Process 2 terminated\n";
                perticularCodeAfterProcess2Terminated();
                wait(NULL);
            }
            else {
                wait(NULL);
                cout << "Process 2 terminated\n";
                perticularCodeAfterProcess2Terminated();
            }
        }
        else {
            //p3
            system("c++ p3.cpp -o p3.exe");
            char* binaryPath = "./p3.exe";
            char* args[] = { binaryPath, NULL };
            execv(binaryPath, args);
            

        }
    }
    else {
        //p2
        system("c++ p2.cpp -o p2.exe");
        char* binaryPath = "./p2.exe";
        char* args[] = { binaryPath, NULL };
        execv(binaryPath, args);
        
    }
    system("rm -f p3.exe");
    system("rm -f p2.exe");
    return 0;
}