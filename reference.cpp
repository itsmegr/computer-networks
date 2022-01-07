#include <unistd.h>
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
    int pid;
    int status;
    pid = fork();
    if (pid > 0) {
        cout<<"this is parent process\n";
        int returnedProgramPid = wait(&status);
        cout << "this is parent process after child with pid "<< "returnedProgramPid" << " exited\n";

    } else {
        cout << "this is child process "<< getpid() <<"\n";
        // sleep(3);
        /*
            execl(const char *path, const char *arg, …, NULL);
            execv(const char *path, char *const argv[]);
            execle(const char *path, const char *arg, …, NULL, char * const envp[] );
            execve(const char *file, char *const argv[], char *const envp[]);
        */
        cout << "this is child process " << getpid() << " after timer\n";
    }
    return 0;
}