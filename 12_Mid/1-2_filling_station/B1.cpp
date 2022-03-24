#include "helper.h"
int d1_fd_uds;


int main() {
    d1_fd_uds = getActiveUDSsocketAfterConnecting("./uds.sock");
    cout << "Connected to the D1\n";

    while (true) {
        cout<<"Waiting for customer\n";
        int client_fd = RecFD(d1_fd_uds);
        cout << "customer received to the B1\n";

        int mqId = initializeMQ("MQ1");
        string amount = recFromMQ(mqId, 1);
        string message = "Pay the amount " + amount + "\n";
        cout << "Take " << amount << " From this customer" << endl;

        WriteFD(client_fd, message);
        cout << ReadFD(client_fd) << " Amount paid" << endl;
    }


}