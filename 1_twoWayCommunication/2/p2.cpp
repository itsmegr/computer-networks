#include <unistd.h>
#include<fcntl.h> 
#include <iostream>
using namespace std;

int main() {
    //restore stdin and stdout
    int reader = dup(0);
    int writer = dup(1);

    int stdout = open("/dev/tty", O_RDWR);
    int stdin = open("/dev/tty", O_RDWR);

    dup2(stdin, 0);
    dup2(stdout, 1);


    while (true) {
        char inpMsg[100];
        int n = read(reader, inpMsg, 100);

        if (strlen(inpMsg) == 0) break;

        cout << "In child, Parent: " << inpMsg << "\n";

        //writing message to parent
        char outMsg[100];
        cout << "In child, Enter message for parent! \n";
        cin.getline(outMsg, 100);
        write(writer, outMsg, strlen(outMsg) + 1);
        if (strlen(outMsg) == 0) break;
    }
    cout << "Shutting down parent\n";
    system("rm -f p2.exe");
    exit(0);
}

