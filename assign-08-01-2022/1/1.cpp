#include <unistd.h>
#include <iostream>
using namespace std;


void parent(int reader, int writer){
    while(true){
        char outMsg[100];
        cout << "In parent, Enter message for child! \n";
        cin.getline(outMsg, 100);
        //whenever you write to fd write with end line character to get the correct output
        write(writer, outMsg, strlen(outMsg)+1);

        if (strlen(outMsg) == 0) break;

        char inpMsg[100];
        read(reader, inpMsg, 100);
        if (strlen(inpMsg)==0) break;
        cout << "In parent, Child: " << inpMsg << endl;
    }

    cout << "Shutting down child\n";
    exit(0);

}

void child(int reader, int writer){
    while(true){
        char inpMsg[100];
        int n = read(reader, inpMsg, 100);

        if (strlen(inpMsg)==0) break;
        
        cout << "In child, Parent: " << inpMsg << "\n";

        //writing message to parent
        char outMsg[100];
        cout << "In child, Enter message for parent! \n";
        cin.getline(outMsg, 100);
        write(writer, outMsg, strlen(outMsg)+1);
        if (strlen(outMsg) == 0) break;
    }

    cout<<"Shutting down parent\n";
    exit(0);
}

int main(){
    //fd[0] is read end(for input in program), fd[1] is write end(for output from program)
    int pipe1fds[2];
    int pipe2fds[2];

    pipe(pipe1fds);
    pipe(pipe2fds);

    int pid = fork();
    if(pid > 0){
        //for parent pipe 1 for input and pipe 2 for output
        close(pipe1fds[1]);
        close(pipe2fds[0]);
        parent(pipe1fds[0], pipe2fds[1]);
    }
    else{
        //for child pipe 2 for input and pipe 1 for output
        close(pipe1fds[0]);
        close(pipe2fds[1]);
        child(pipe2fds[0], pipe1fds[1]);
    }

}