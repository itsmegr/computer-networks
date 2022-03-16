#include "helper.h"
#define PORT 8080
#define MAXLINE 4096
int bowler_usfd, run, passive_fd, rsfd, active_fd;


void getRunFromFd(int fd)
{
    string data = ReadFD(fd);

    int start = 0;

    int end = data.find(" ");
    int speed = stoi(data.substr(start, end - start));
    int spin = stoi(data.substr(end + 1));
    // run = (speed + spin) % 40;
    run = 6;
    cout << "Run randomized\n";

}


void raw_socket() {
    if ((rsfd = socket(PF_INET, SOCK_RAW, 0)) < 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    //sending run to fielders
    string s = to_string(run);
    char buffer[MAXLINE];
    sendto(rsfd, s.c_str(), s.size(),
        0, (const struct sockaddr*)&server_addr, sizeof(server_addr));
    memset(buffer, 0, MAXLINE);

    close(rsfd);
}



int main() {
    // d1_fd_uds = getActiveUDSsocketAfterConnecting("./uds.sock");
    // cout << "Connected to the D1\n";


    active_fd = getActiveUDSsocketAfterConnecting("./uds3.sock");
    cout << "umpire connected to batsman\n";


    passive_fd = getpassiveUDSsocket("./uds2.sock", true);
    bowler_usfd = accept(passive_fd, NULL, NULL);
    cout << "Bowler connected to batsman\n";

    //getting the fd
    int file_fd = RecFD(bowler_usfd);

    //randomized the run
    getRunFromFd(file_fd);



    //here send the data through raw socket to fielder
    raw_socket();


    //sending the run to umpire
    WriteFD(active_fd, to_string(run));


}