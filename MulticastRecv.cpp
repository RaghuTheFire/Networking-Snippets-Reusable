
#include <iostream>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#define MULTICAST_ADDR "224.0.0.1"
#define MULTICAST_PORT 8888
#define BUFFER_SIZE 1024

int main() 
{
    int sock;
    struct sockaddr_in addr;
    char buffer[BUFFER_SIZE];
    socklen_t addrlen;
    int bytes_read;

    // Create a UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) 
    {
        std::cerr << "Failed to create socket" << std::endl;
        return 1;
    }

    // Set up the multicast group address
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(MULTICAST_ADDR);
    addr.sin_port = htons(MULTICAST_PORT);

    // Join the multicast group
    struct ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = inet_addr(MULTICAST_ADDR);
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) 
    {
        std::cerr << "Failed to join multicast group" << std::endl;
        close(sock);
        return 1;
    }

    // Receive multicast data
    while (true) 
    {
        addrlen = sizeof(addr);
        bytes_read = recvfrom(sock, buffer, BUFFER_SIZE - 1, 0, (struct sockaddr*)&addr, &addrlen);
        if (bytes_read < 0) 
        {
            std::cerr << "Failed to receive data" << std::endl;
            break;
        }

        buffer[bytes_read] = '\0';
        std::cout << "Received: " << buffer << std::endl;
    }

    // Leave the multicast group
    if (setsockopt(sock, IPPROTO_IP, IP_DROP_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) 
    {
        std::cerr << "Failed to leave multicast group" << std::endl;
    }

    close(sock);
    return 0;
}
