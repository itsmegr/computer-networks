#include <unistd.h>
#include<fcntl.h> 
#include <iostream>
using namespace std;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wc++11-compat-deprecated-writable-strings"


void forward(int readerFromP1, int writerToP3, int readerFromP3, int writerToP1) {
    while(true){
        //listenening for sending to p3
        char msgForP3[100];
        read(readerFromP1, msgForP3, 100);

        //forward to p3
        write(writerToP3, msgForP3, strlen(msgForP3) + 1);
        if (strlen(msgForP3) == 0) break;


        //listenening for sending to p1
        char msgForP1[100];
        read(readerFromP3, msgForP1,100);

        //forward to p1
        write(writerToP1, msgForP1, strlen(msgForP1)+1);
        if (strlen(msgForP1) == 0) break;
    }


}

int main() {
    //restore stdin and stdout
    int readerFromP1 = dup(0);
    int writerToP1 = dup(1);

    int stdout = open("/dev/tty", O_RDWR);
    int stdin = open("/dev/tty", O_RDWR);

    dup2(stdin, 0);
    dup2(stdout, 1);


    int pipe1fds[2];
    int pipe2fds[2];

    pipe(pipe1fds);
    pipe(pipe2fds);

    int pid = fork();
    if (pid > 0) {
        //for parent pipe 1 for input and pipe 2 for output
        close(pipe1fds[1]);
        close(pipe2fds[0]);

        forward(readerFromP1, pipe2fds[1], pipe1fds[0], writerToP1);
    }
    else {
        cout << "In P3, I am process 3\n";
        //for child pipe 2 for input and pipe 1 for output
        close(pipe1fds[0]);
        close(pipe2fds[1]);
        // child(pipe2fds[0], pipe1fds[1]);

        dup2(pipe2fds[0], 0);
        dup2(pipe1fds[1], 1);

        system("c++ p3.cpp -o p3.exe");
        char* binaryPath = "./p3.exe";
        char* args[] = { binaryPath, NULL };
        execv(binaryPath, args);
    }

    cout << "shutting down P2" << endl;
    system("rm -f p2.exe");
}
#pragma GCC diagnostic pop

