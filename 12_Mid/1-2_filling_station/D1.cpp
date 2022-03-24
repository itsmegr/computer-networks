#include "helper.h"
int uds_socket_fd, b1_fd, bsd_socket_fd;


int main() {
   uds_socket_fd =  getpassiveUDSsocket("./uds.sock", true);
   b1_fd = accept(uds_socket_fd, NULL, NULL);
   cout<<"D1 connected\n";


   int  D1_PORT = 8080;
   bsd_socket_fd = getSocket(SOCK_STREAM);
   struct sockaddr_in connectionSocketAddress = bindSocketToPort(D1_PORT, bsd_socket_fd);
   listenForSocket(bsd_socket_fd);

   while(true){
       cout<<"Waiting for customers to come \n";
       // accept the incoming connection
       struct sockaddr_in client_address;
       int client_sfd;
       client_address = acceptConn(bsd_socket_fd, &client_sfd);

       WriteFD(client_sfd, "For how much amount you want petrol\n");

       string amount = ReadFD(client_sfd);
       cout << "okk filled petrol for " << amount << " amount\n";
       cout << "Now go to the billing counter and pay the bill\n";

       //fd sent to the B1
       SendFD(b1_fd, client_sfd);

       //now informing B1 to take the appropriate amount
       int mqId = initializeMQ("MQ1");

       sendInMQ(mqId, 1, amount);
       cout << "customer sent to the billing counter \n";
   }
}