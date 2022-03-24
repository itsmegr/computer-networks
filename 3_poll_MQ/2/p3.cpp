#include <stdio.h>
#include <iostream>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
using namespace std;

char FIFO3[10] = "FIFO3";


int main() {
    int ffd = open(FIFO3, O_WRONLY);
    int pid = getpid();
    cout << "Pid is " << pid << endl;

    while (true) {
        char message[100];
        cout << "P3, Enter message: ";
        cin.getline(message, 100);
        //compare message and exit
        if (strcmp(message, "exit") == 0) {
            write(ffd, message, strlen(message) + 1);
            break;
        }
        //form a new string 
        char new_message[100];
        sprintf(new_message, "%d: %s", pid, message);
        write(ffd, new_message, strlen(new_message) + 1);
    }
    printf("terminating");
    close(ffd);
    exit(0);
}