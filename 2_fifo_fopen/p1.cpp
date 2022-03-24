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

int file_fd_for_writing;
char FIFO_NAME[100] = "CAPATALIZED_DATA";

//thread funtion for reading input from user
void* read_input_from_user(void* arg) {
    cout << "Speaker thread started" << endl;
    
    while (true) {
        char buf[100];
        cin.getline(buf, 100);

        write(file_fd_for_writing, buf, strlen(buf)+1);

        if (strlen(buf) == 0) {
            close(file_fd_for_writing);
            //terminate thread
            pthread_exit(NULL);
        }
    }
}

struct struct1
{
   int fd;
};



//thread for reading input from p3 using named pipe/FiFO
void* read_input_from_P3(void* arg) {
    cout << "Listener thread started" << endl;
    int ffd = open(FIFO_NAME, O_RDONLY);

    while(true){
        char buff[100];
        read(ffd, buff, 100);
        if (strlen(buff) == 0) {
            close(ffd);
            break;
        }
        cout <<"CAPATALIZED: " << buff << endl;
    }
    return NULL;
}


int main() {

    system("c++ p2.cpp -o p2.exe");

    // use popen to execute p2.exe
    FILE* fp = popen("./p2.exe", "w");

    //create a thread for reading input from user
    pthread_t tid1;
    file_fd_for_writing = fileno(fp);
    pthread_create(&tid1, NULL, read_input_from_user, NULL);

    //make fifo 
    mkfifo(FIFO_NAME, 0777);


    //create a thread for reading input from p3 using named pipe/FiFO
    pthread_t tid2;
    struct1 obj;
    obj.fd = 12;
    pthread_create(&tid2, NULL, read_input_from_P3, (void*)&obj);


    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    cout << "Process 1 terminated\n";
    //create a thread for continously taking input from user and sending to p2.exe
    pclose(fp);
    exit(0);

}