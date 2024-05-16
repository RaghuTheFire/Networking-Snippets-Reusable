
#include <iostream>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MULTICAST_IP "224.0.0.1"
#define MULTICAST_PORT 8888

int main() 
{
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) 
    {
        std::cerr << "Failed to create socket" << std::endl;
        return 1;
    }

    struct sockaddr_in multicastAddr;
    memset(&multicastAddr, 0, sizeof(multicastAddr));
    multicastAddr.sin_family = AF_INET;
    multicastAddr.sin_addr.s_addr = inet_addr(MULTICAST_IP);
    multicastAddr.sin_port = htons(MULTICAST_PORT);

    const char* message = "Hello, Multicast!";
    int len = strlen(message);

    while (true) 
    {
        int sent = sendto(sock, message, len, 0, (struct sockaddr*)&multicastAddr, sizeof(multicastAddr));
        if (sent < 0) 
        {
            std::cerr << "Failed to send message" << std::endl;
            break;
        }
        std::cout << "Sent message: " << message << std::endl;
        sleep(1);
    }

    close(sock);
    return 0;
}
