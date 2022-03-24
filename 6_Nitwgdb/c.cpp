#include <stdio.h>
#include <iostream>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <semaphore.h>
using namespace std;

//function to send the file code to server
void SendFile(int sfd, string s)
{
    s.push_back('\0');
    send(sfd, s.c_str(), s.size(), 0);
}

//reading code frm file
string ReadFile(string filename)
{
    string s = "";
    int fd = open(filename.c_str(), O_RDWR, 0777);
    while (true)
    {
        char c;
        int a = read(fd, &c, 1);
        if (a <= 0)
        {
            break;
        }
        s.push_back(c);
    }
    close(fd);
    return s;
}



int main()
{
    string codeForFile = ReadFile("p.cpp");
    string expectedInput = ReadFile("IT.txt");
    string expectedOutput = ReadFile("OT.txt");

    // opening socket client
    int PORT = 8080;
    int sfd, valread;
    struct sockaddr_in address;
    char buffer[1024] = {0};

    // socket file descriptor
    // getting a socket
    if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    cout << "Socket created" << endl;

    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &address.sin_addr) <= 0)
    {
        perror("inet_pton");
        exit(EXIT_FAILURE);
    }

    cout << "Connecting to the server ..." << endl;

    // connect the client socket to server socket
    while (connect(sfd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("connect");
        cout << "Connection failed error occured, retrying in 2 seconds" << endl;
        sleep(2);
    }

    cout << "Connection established...\n";
    cout << "Sending code to server..." << endl;

    SendFile(sfd, codeForFile);
    SendFile(sfd, expectedInput);
    SendFile(sfd, expectedOutput);

    cout << "code sent..." << endl;

    read(sfd, buffer, 1024);
    string msg(buffer);
    cout << msg << endl;

    close(sfd);

    return 0;
}