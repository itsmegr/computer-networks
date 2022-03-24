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
#include <sys/un.h>
#define MSG_CMSG_CLOEXEC 0x40000000
using namespace std;

//this is helper file containing all the helper functions
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

string ReadFD(int fd)
{
    string s = "";
    char ch;
    while (true)
    {
        int n = read(fd, &ch, 1);
        if (n <= 0 || ch == '\0')
        {
            break;
        }
        s.push_back(ch);
    }
    return s;
}

int WriteFD(int fd, string s)
{
    s.push_back('\0');
    return send(fd, s.c_str(), s.size(), 0);
}

struct msg_st
{
    long int type;
    char msg_text[128];
};


int initializeMQ(string mqName) {

    int key = ftok(mqName.c_str(), 65);
    int mqId = msgget(key, 0666 | IPC_CREAT);
    return mqId;
}

void sendInMQ(int mqId, long int msgType, string message) {
    message.push_back('\0');
    msg_st msg;
    msg.type = msgType;
    strcpy(msg.msg_text, message.c_str());
    msgsnd(mqId, &msg, sizeof(msg.msg_text), 0);
}

string recFromMQ(int mqId, long int msgType = 0) {
    msg_st msg;

    msgrcv(mqId, &msg, sizeof(msg.msg_text), msgType, 0);

    return string(msg.msg_text);
}


//code regarding uds
int getpassiveUDSsocket(string filename, bool doUnlink)
{
    struct sockaddr_un addr2;
    int sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (doUnlink)
        unlink(filename.c_str());
    memset(&addr2, 0, sizeof(struct sockaddr_un));
    strncpy(addr2.sun_path, filename.c_str(), sizeof(addr2.sun_path) - 1);
    bind(sfd, (struct sockaddr*)&addr2, sizeof(addr2));
    listen(sfd, SOMAXCONN);
    cout << "listening at Uds server..." << endl;
    return sfd;
}


int getActiveUDSsocketAfterConnecting(string fileName) {
    struct sockaddr_un addr;
    int sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    memset(&addr, 0, sizeof(struct sockaddr_un));
    strncpy(addr.sun_path, fileName.c_str(), sizeof(addr.sun_path) - 1);
    bind(sfd, (struct sockaddr*)&addr, sizeof(addr));

    if (connect(sfd, (struct sockaddr*)&addr, sizeof(struct sockaddr_un)) == -1)
    {
        cout << "Error connecting to uds server..." << endl;
        exit(1);
    }

    cout << "Connected to Uds server..." << endl;
    return sfd;
}


void SendFD(int socket, int fd)
{
    struct msghdr socket_message;
    struct iovec io_vector[1];
    struct cmsghdr* control_message = NULL;
    char message_buffer[1];
    /* storage space needed for an ancillary element with a payload of length is CMSG_SPACE(sizeof(length)) */
    char ancillary_element_buffer[CMSG_SPACE(sizeof(int))];
    int available_ancillary_element_buffer_space;

    /* at least one vector of one byte must be sent */
    message_buffer[0] = 'F';
    io_vector[0].iov_base = message_buffer;
    io_vector[0].iov_len = 1;

    /* initialize socket message */
    memset(&socket_message, 0, sizeof(struct msghdr));
    socket_message.msg_iov = io_vector;
    socket_message.msg_iovlen = 1;

    /* provide space for the ancillary data */
    available_ancillary_element_buffer_space = CMSG_SPACE(sizeof(int));
    memset(ancillary_element_buffer, 0, available_ancillary_element_buffer_space);
    socket_message.msg_control = ancillary_element_buffer;
    socket_message.msg_controllen = available_ancillary_element_buffer_space;

    /* initialize a single ancillary data element for fd passing */
    control_message = CMSG_FIRSTHDR(&socket_message);
    control_message->cmsg_level = SOL_SOCKET;
    control_message->cmsg_type = SCM_RIGHTS;
    control_message->cmsg_len = CMSG_LEN(sizeof(int));
    *((int*)CMSG_DATA(control_message)) = fd;

    sendmsg(socket, &socket_message, 0);
}

int RecFD(int socket)
{
    int sent_fd, available_ancillary_element_buffer_space;
    struct msghdr socket_message;
    struct iovec io_vector[1];
    struct cmsghdr* control_message = NULL;
    char message_buffer[1];
    char ancillary_element_buffer[CMSG_SPACE(sizeof(int))];

    /* start clean */
    memset(&socket_message, 0, sizeof(struct msghdr));
    memset(ancillary_element_buffer, 0, CMSG_SPACE(sizeof(int)));

    /* setup a place to fill in message contents */
    io_vector[0].iov_base = message_buffer;
    io_vector[0].iov_len = 1;
    socket_message.msg_iov = io_vector;
    socket_message.msg_iovlen = 1;

    /* provide space for the ancillary data */
    socket_message.msg_control = ancillary_element_buffer;
    socket_message.msg_controllen = CMSG_SPACE(sizeof(int));

    if (recvmsg(socket, &socket_message, MSG_CMSG_CLOEXEC) < 0)
        return -1;

    if (message_buffer[0] != 'F')
    {
        /* this did not originate from the above function */
        return -1;
    }

    if ((socket_message.msg_flags & MSG_CTRUNC) == MSG_CTRUNC)
    {
        /* we did not provide enough space for the ancillary element array */
        return -1;
    }

    /* iterate ancillary elements */
    for (control_message = CMSG_FIRSTHDR(&socket_message);
        control_message != NULL;
        control_message = CMSG_NXTHDR(&socket_message, control_message))
    {
        if ((control_message->cmsg_level == SOL_SOCKET) &&
            (control_message->cmsg_type == SCM_RIGHTS))
        {
            sent_fd = *((int*)CMSG_DATA(control_message));
            return sent_fd;
        }
    }

    return -1;
}