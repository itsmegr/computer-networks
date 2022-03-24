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

int getSocket(int protocolType) {
    // socket file descriptor
    // getting a socket
    /*
        AF_INET :- Address Family IPv4
        SOCK_STREAM :- TCP Protocol
    */
    int socketfd = socket(AF_INET, protocolType, 0);
    if (socketfd == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    cout << "Created socket.." << endl;

    //setsoctopt to reuse the port
    int opt = 1;
    if (setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    return socketfd;
}


struct sockaddr_in bindSocketToPort(int port, int sfd) {
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
    address.sin_port = htons(port);

    // binding the address to sfd
    if (bind(sfd, (struct sockaddr*)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    cout << "Binded socket to port: " << port << endl;
    return address;
}

void printIPandPort(struct sockaddr_in address, string whose) {
    char myIP[16];
    inet_ntop(AF_INET, &address.sin_addr, myIP, sizeof(myIP));
    int myPort = ntohs(address.sin_port);
    cout << whose << " IP: " << myIP << endl;
    cout << whose << " Port: " << myPort << endl;
}

void listenForSocket(int sfd) {
    if (listen(sfd, 10) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    cout << "Ready for listening.." << endl;
}

struct sockaddr_in acceptConn(int sfd, int* client_sfd) {
    struct sockaddr_in client_address;
    int client_addrlen = sizeof(client_address);
    *client_sfd = accept(sfd, (struct sockaddr*)&client_address, (socklen_t*)&client_addrlen);
    if (*client_sfd < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    cout << "Accepted connection.." << endl;
    return client_address;
}

struct sockaddr_in connectToServer(char* serverIp, int serverPort, int sfd) {
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(serverPort);
    /*
        The inet_pton() function converts an Internet server_address in its standard text format
        into its numeric binary form
    */
    if (inet_pton(AF_INET, serverIp, &server_address.sin_addr) <= 0)
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
    return server_address;
}


void socket_util() {
    int sfd, SERVER_PORT = 8080, PORT = 8081;
    sfd = getSocket(SOCK_STREAM);
    struct sockaddr_in address;
    address  = bindSocketToPort(PORT,sfd);
    printIPandPort(address, "Server");

    struct sockaddr_in server_address = connectToServer("127.0.0.1", SERVER_PORT, sfd);

    //send message to server
    char message[100];
    cout << "Enter message: ";
    cin.getline(message, 100);
    send(sfd, message, strlen(message) +1, 0);
    cout << "Message sent to server.." << endl;
    close(sfd);
}

int main()
{
    socket_util();
    return 0;
}