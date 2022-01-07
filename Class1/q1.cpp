#include <unistd.h>
#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
    int pid, status;
    pid = fork();
    if (pid > 0) {
        cout << "this is parent process\n";
        int returnedProgramPid = wait(&status);
        cout << "this is parent process after child with pid " << returnedProgramPid << " exited\n";

    }
    else {

        cout << "this is child process " << getpid() << "\n";

        system("c++ p2.cpp -o p2.exe");
        char* binaryPath = "./p2.exe";
        char* args[] = { binaryPath, NULL };
        execv(binaryPath, args);


        cout << "this is child process " << getpid() << " after timer\n";
    }
    system("rm -f p2.exe");
    return 0;
}