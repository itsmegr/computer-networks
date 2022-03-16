#include "helper.h"
int batsman_usfd, passive_fd, active_fd;


int main() {
    passive_fd = getpassiveUDSsocket("./uds2.sock", true);
    batsman_usfd = accept(passive_fd, NULL, NULL);
    cout << "bastman connected to umpire\n";


    string run = ReadFD(batsman_usfd);
    sleep(5);

    cout << "Run scored by batsman on this ball is " << run << endl;

}