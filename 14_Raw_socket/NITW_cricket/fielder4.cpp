#include "helper.h"

#define PORT 8081
#define MAXLINE 4096
int active_usfd, file_fd;


int main() {
    //just need to receive the the message
    int sfd;

    if ((sfd = socket(PF_INET, SOCK_RAW, 0)) < 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(sfd, (const struct sockaddr*)&addr,
        sizeof(addr));


    struct sockaddr_in clientAddr;
    int len = sizeof(clientAddr);
    memset(&clientAddr, 0, sizeof(clientAddr));

    cout << "Fielder Ready for fielding\n";

    unsigned char buffer[MAXLINE];
    // this will not only rec message but also the client add
    int packetSize = recvfrom(sfd, (char*)buffer, MAXLINE,
        0, (sockaddr*)&clientAddr,
        (socklen_t*)&len);
    if (packetSize <= 0)
    {

    }
    unsigned char tempBuff[MAXLINE];
    for (int i = 0; i < MAXLINE; i++)
        tempBuff[i] = buffer[i];

    struct ip* iph = (struct ip*)buffer;

    cout << "Run scored:\n";
    printPayload(iph, tempBuff, packetSize);

    memset(buffer, 0, MAXLINE);
    return 0;
}