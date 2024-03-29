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
#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <pthread.h>
using namespace std;
#define PORT 8080
#define MAXLINE 1024
#define MSG_CONFIRM 0x800
int main()
{
    int sfd;
    if ((sfd = socket(PF_INET, SOCK_RAW, 0)) < 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    while (true)
    {
        string s = "";
        char buffer[MAXLINE];
        cout << "Enter text : ";
        getline(cin, s);
        sendto(sfd, s.c_str(), s.size(),
               0, (const struct sockaddr *)&server_addr, sizeof(server_addr));
        memset(buffer, 0, MAXLINE);
    }

    close(sfd);
    return 0;
}