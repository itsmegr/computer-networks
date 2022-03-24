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


char FIFO_NAME[100] = "CAPATALIZED_DATA";
int main() {
    cout << "Starting p3\n";
    int ffd = open(FIFO_NAME, O_WRONLY);

    while (true) {
        char buff[100];
        read(0, buff, 100);

        for (int i = 0; i < strlen(buff); i++) {
            buff[i] = toupper(buff[i]);
        }

        write(ffd, buff, strlen(buff) + 1);
        if (strlen(buff) == 0) {
            close(ffd);
            break;
        }
        
    }
    cout << "Process 3 terminated\n";
    system("rm -f p3.exe");
    exit(0);
}