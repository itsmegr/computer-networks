#include <unistd.h>
#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
    int pid2, pid3, status;
    pid2 = fork();
    if (pid2 > 0) {
        pid3 = fork();
        if(pid3 > 0){
            //parent
            int firstReturnedProcessPID = wait(&status);
            if(firstReturnedProcessPID==pid2){
                cout<<"Process 2 exited first\n";
                wait(NULL);
                cout << "Process 3 exited second\n";

            }
            else{
                cout << "Process 3 exited first\n";
                wait(NULL);
                cout << "Process 2 exited second\n";
            }

        }
        else{
            //p3
            char* binaryPath = "./p3.exe";
            char* args[] = { binaryPath, NULL };
            execv(binaryPath, args);

        }
    }
    else {
        //p2
        char* binaryPath = "./p2.exe";
        char* args[] = { binaryPath, NULL };
        execv(binaryPath, args);
    }
    return 0;
}