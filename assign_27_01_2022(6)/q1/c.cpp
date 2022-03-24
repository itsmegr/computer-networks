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
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <bits/stdc++.h>
#include <arpa/inet.h>
using namespace std;

int sfd, SERVER_PORT = 8080, PORT=8081;

void socket_util() {
    // socket file descriptor
    // getting a socket
    /*
        AF_INET :- Address Family IPv4
        SOCK_STREAM :- TCP Protocol
    */
    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    cout << "Created socket.." << endl;

    //setsoctopt to reuse the port
    int opt = 1;
    if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }


    struct sockaddr_in address;
    int addrlen = sizeof(address);


    /*
        sin_family :- Address Family IPv4
        sin_port :- Port Number
        sin_addr :- IP Address
    */


    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;


    /*
        The htons() function converts the unsigned short integer hostshort from host
        byte order to network byte order.
        The ntohs() function converts the unsigned short integer netshort from network
        byte order to host byte order.
    */
    address.sin_port = htons(PORT);



    // binding the address to sfd
    if (bind(sfd, (struct sockaddr*)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    cout << "Binded socket to address.." << endl;

    char myIP[16];
    inet_ntop(AF_INET, &address.sin_addr, myIP, sizeof(myIP));
    int myPort = ntohs(address.sin_port);
    cout << "Local IP: " << myIP << endl;
    cout << "Local Port: " << myPort << endl;


    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);
    /*
        The inet_pton() function converts an Internet server_address in its standard text format
        into its numeric binary form
    */
    if (inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) <= 0)
    {
        perror("inet_pton");
        exit(EXIT_FAILURE);
    }

    cout << "Connecting to the server ..." << endl;
    // connect the client socket to server socket
    while (connect(sfd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0)
    {
        perror("connect");
        cout << "Connection failed, retrying in 5 seconds" << endl;
        sleep(5);
    }
    cout << "Connection established...\n";


    //send message to server
    char message[100];
    cout << "Enter message: ";
    cin.getline(message, 100);
    send(sfd, message, strlen(message), 0);
    cout << "Message sent to server.." << endl;


    close(sfd);


}

int main()
{
    socket_util();
    return 0;
}