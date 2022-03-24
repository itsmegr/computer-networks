#include "helper.h"


int main() {
    int PORT;
    cout<<"Enter the port Number\n";
    cin>>PORT;
    
    int sfd, SERVER_PORT = 8080;
    sfd = getSocket(SOCK_STREAM);
    struct sockaddr_in address;
    address = bindSocketToPort(PORT, sfd);
    struct sockaddr_in server_address = connectToServer("127.0.0.1", SERVER_PORT, sfd);
    cout << "Connected to dispenser\n";

    string msg;
    cout << ReadFD(sfd);
    cin >> msg;
    WriteFD(sfd, msg);


    cout << ReadFD(sfd);
    cin >> msg;
    WriteFD(sfd, msg);

}