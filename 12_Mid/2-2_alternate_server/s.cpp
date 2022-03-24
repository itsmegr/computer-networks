#include "helper.h"
bool isMMode = false;
void *Server(void *arg)
{
    while (1)
    {
        struct sockaddr_in address;
        int addrlen = sizeof(address);
        int nsfd;
        // accept the incoming connection and get nsfd
        if ((nsfd = accept(sfd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        cout << "New client connected in S server..." << endl;
        int pid = GetPID(nsfd);
        cout << "PID: " << pid << endl;

        pthread_t client_thread;
        pthread_create(&client_thread, NULL, ClientHandler, (void *)&nsfd);

        pids.push_back(pid);
        nsfds.push_back(nsfd);
        tids.push_back(client_thread);
    }
    return NULL;
}
int main()
{
    //signal handler for closing all the fds
    signal(SIGINT, CloseHandler);

    sfd = ServerInit(8080);

    //connecting to the As server
    asfd = ConnectToServer("127.0.0.1", 3000);
    if (listen(sfd, SOMAXCONN) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    //handling the main server in one thread
    pthread_t id;
    pthread_create(&id, NULL, Server, NULL);

    //here continuously asking the user to go in maintenance mode
    do
    {
        cout << "Mode: " << (!isMMode ? "Working" : "Maintenance")
             << " Toggle? y/n";
        char ch;
        cin >> ch;
        isMMode = ch == 'y' ? !isMMode : isMMode;
        MaintainanceMode(isMMode);

    } while (true);
    return 0;
}