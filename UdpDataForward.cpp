#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main() 
{
    int udpSocket;
    struct sockaddr_in serverAddr, clientAddr, forwardAddr;
    char buffer[BUFFER_SIZE];
    socklen_t addrLen = sizeof(clientAddr);

    udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (udpSocket < 0) 
    {
        std::cerr << "Socket creation failed" << std::endl;
        return -1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8080);

    if (bind(udpSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) 
    {
        std::cerr << "Bind failed" << std::endl;
        close(udpSocket);
        return -1;
    }

    forwardAddr.sin_family = AF_INET;
    forwardAddr.sin_addr.s_addr = inet_addr("192.168.1.100"); // Change to the target IP
    forwardAddr.sin_port = htons(9090); // Change to the target port

    while (true) 
    {
        ssize_t recvLen = recvfrom(udpSocket, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&clientAddr, &addrLen);
        if (recvLen < 0) 
        {
            std::cerr << "Receive failed" << std::endl;
            break;
        }

        sendto(udpSocket, buffer, recvLen, 0, (struct sockaddr*)&forwardAddr, sizeof(forwardAddr));
    }

    close(udpSocket);
    return 0;
}
