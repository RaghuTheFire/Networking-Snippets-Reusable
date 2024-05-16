
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

const int MAX_PORTS = 65535;

bool scan_port(const char* ip, int port) 
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) 
    {
        cerr << "Failed to create socket" << endl;
        return false;
    }

    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &addr.sin_addr);

    if (connect(sock, (sockaddr*)&addr, sizeof(addr)) == 0) 
    {
        cout << "Port " << port << " is open" << endl;
        close(sock);
        return true;
    } 
    else 
    {
        close(sock);
        return false;
    }
}

int main(int argc, char* argv[]) 
{
    if (argc != 2) 
    {
        cerr << "Usage: " << argv[0] << " <ip_address>" << endl;
        return 1;
    }

    const char* ip = argv[1];
    vector<int> open_ports;

    for (int port = 1; port <= MAX_PORTS; port++) 
    {
        if (scan_port(ip, port)) 
        {
            open_ports.push_back(port);
        }
    }

    cout << "Open ports on " << ip << ":" << endl;
    for (int port : open_ports) 
    {
        cout << port << " ";
    }
    cout << endl;

    return 0;
}
