#include <stdio.h>
#include <iostream>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>

using namespace std;
char FIFO2[10] = "FIFO2";
char FIFO3[10] = "FIFO3";
char FIFO4[10] = "FIFO4";
struct pollfd fds[3];


int main(){
    mkfifo(FIFO2, 0777);
    mkfifo(FIFO3, 0777);
    mkfifo(FIFO4, 0777);


    int ffd2, ffd3, ffd4;
    ffd2 = open(FIFO2, O_RDWR);
    ffd3 = open(FIFO3, O_RDWR);
    ffd4 = open(FIFO4, O_RDWR);

    fds[0].fd = ffd2;
    fds[1].fd = ffd3;
    fds[2].fd = ffd4;

    fds[0].events = POLLIN;
    fds[1].events = POLLIN;
    fds[2].events = POLLIN;

    cout<<"P1 is ready to POLL"<<endl;

    while(true){
        int ret = poll(fds, 3, -1);
        if(ret == -1){
            perror("poll");
            exit(1);
        }
        if(fds[0].revents & POLLIN){
            char message[100];
            read(ffd2, message, 100);
            if(strcmp(message, "exit")==0) break;
            printf("%s\n", message);
        }
        if(fds[1].revents & POLLIN){
            char message[100];
            read(ffd3, message, 100);
            if (strcmp(message, "exit") == 0) break;
            printf("%s\n", message);
        }
        if(fds[2].revents & POLLIN){
            char message[100];
            read(ffd4, message, 100);
            if (strcmp(message, "exit") == 0) break;
            printf("%s\n", message);
        }
    }

    printf("Any of the process choose to exit, terminating");

    // close(ffd2);
    // close(ffd3);
    // close(ffd4);
}