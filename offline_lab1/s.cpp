#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/wait.h>
#include <pthread.h>
#include <poll.h>
#include <sys/poll.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <ctype.h>
#include <string.h>
#include <pthread.h>
#include <sys/poll.h>
#include <sys/time.h>
#include<fstream>
#define MAXLINE 1024
using namespace std;
int main() {
    int rsfd;
    if ((rsfd = socket(PF_INET, SOCK_RAW, 0)) < 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(3105);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");





    int sfd, nsfd;
    struct sockaddr_in serv_addr, cli_addr;
    int portno = 6097;
    char* ip = "127.0.0.1";
    char buff[1024];
    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&serv_addr, sizeof(serv_addr));
    int opt = 1;
    if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR,
        &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(ip);
    serv_addr.sin_port = htons(portno);

    if ((bind(sfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");
    listen(sfd, 3);
    socklen_t  cli_len = sizeof(cli_addr);

    nsfd = accept(sfd, (struct sockaddr*)&cli_addr, &cli_len);
    if (nsfd < 0) {
        printf("server accept failed...\n");
        exit(0);
    }
    else
        cout << "Connection request accepted by server.\n";

    int cnt = 0;
    while (cnt < 5) {
        bzero(buff, sizeof(buff));
        recv(nsfd, &buff, sizeof(buff), 0);
        cout << "ESS server recvd: " << buff << "\n";
        bzero(buff, sizeof(buff));
        strcpy(buff, "I am ESS server.");
        send(nsfd, &buff, sizeof(buff), 0);


        string s = "";
        char buffer[MAXLINE];
        cout << "Enter Message to be sent to all : ";
        getline(cin, s);
        sendto(rsfd, s.c_str(), s.size(),
            0, (const struct sockaddr*)&server_addr, sizeof(server_addr));
        memset(buffer, 0, MAXLINE);
        cnt++;
    }
}