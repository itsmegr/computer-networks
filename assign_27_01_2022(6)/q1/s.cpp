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

int sfd, PORT=8080;

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

    /********************************************************************/
      /* The listen() function allows the server to accept incoming       */
      /* client connections.  In this example, the backlog is set to 10.  */
      /* This means that the system will queue 10 incoming connection     */
      /* requests before the system starts rejecting the incoming         */
      /* requests.                                                        */
    /********************************************************************/
    if (listen(sfd, 10) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    cout<<"Ready for listening.."<<endl;

    // accept the incoming connection
    struct sockaddr_in client_address;
    int client_addrlen = sizeof(client_address);
    int client_sfd = accept(sfd, (struct sockaddr*)&client_address, (socklen_t*)&client_addrlen);
    if (client_sfd < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    cout << "Accepted connection.." << endl;

    //print the address of client
    char clientIP[16];
    inet_ntop(AF_INET, &client_address.sin_addr, clientIP, sizeof(clientIP));
    int clientPort = ntohs(client_address.sin_port);
    cout << "Client IP: " << clientIP << endl;
    cout << "Client Port: " << clientPort << endl;


    //receive the message from client
    char buffer[100];
    int n = recv(client_sfd, buffer, sizeof(buffer), 0);
    if (n < 0)
    {
        perror("recv");
        exit(EXIT_FAILURE);
    }
    cout << "Received message from client: " << buffer << endl;

    close(sfd);
    close(client_sfd);
}

int main()
{
    socket_util();
    return 0;
}