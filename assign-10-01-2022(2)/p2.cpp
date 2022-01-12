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

int main() {
    cout << "Starting p2\n";
    system("c++ p3.cpp -o p3.exe");

    FILE* fp = popen("./p3.exe", "w");
    int file_fd_for_writing = fileno(fp);


    while (true) {
        char buff[100];
        read(0, buff, 100);
        write(file_fd_for_writing, buff, strlen(buff) + 1);
        if (strlen(buff) == 0) {
            pclose(fp);
            pclose(0);
            break;
        }
        
    }

    cout << "Process 2 terminated\n";
    system("rm -f p2.exe");
    exit(0);
}