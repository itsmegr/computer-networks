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

        char* binaryPath = "./p2.exe";
        char* args[] = { binaryPath, NULL };

        execv(binaryPath, args);

        cout << "this is child process " << getpid() << " after timer\n";
    }
    return 0;
}