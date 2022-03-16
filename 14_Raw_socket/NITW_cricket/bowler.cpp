#include "helper.h"
int active_usfd, file_fd;


int main() {
    active_usfd = getActiveUDSsocketAfterConnecting("./uds2.sock");
    cout << "Connected to the batsman\n";

    file_fd = open("file.txt", O_RDONLY);

    SendFD(active_usfd, file_fd);










}