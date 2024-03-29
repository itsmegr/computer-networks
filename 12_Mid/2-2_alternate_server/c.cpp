#include "helper.h"

//that's simple client
int main(int argc, char *argv[])
{
    string pidString = to_string(getpid());
    cout << "My PID: " << pidString << endl;
    int PORT = 8080;
    string ip = "127.0.0.1";
    while (true)
    {
        int sfd = ConnectToServer(ip, PORT);
        WriteFD(sfd, pidString);
        while (true)
        {
            string s = ReadFD(sfd);
            cout << "Server said: " << s << endl;
            if (s == "")
            {
                break;
            }
            //here if message starts with # then Main server is 
            //in maintenance mode so we need to connect it to AS server
            if (s[0] == '#')
            {
                cout << "Got Server Change Signal...\n";
                ServerData data = IntepretServerChangeMessage(s);
                PORT = data.port;
                ip = data.ip;
                printf("IP: %s, PORT: %d\n", ip.c_str(), PORT);
                close(sfd);
                break;
            }
        }
    }

    return 0;
}