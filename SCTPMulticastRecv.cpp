
#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <netinet/sctp.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define MULTICAST_ADDR "239.0.0.1"
#define MULTICAST_PORT 5000

int main() 
{
    int sock_fd, ret;
    struct sockaddr_in addr;
    struct ip_mreq mreq;
    char buffer[BUFFER_SIZE];

    // Create a SCTP socket
    sock_fd = socket(AF_INET, SOCK_SEQPACKET, IPPROTO_SCTP);
    if (sock_fd < 0) 
    {
        std::cerr << "Failed to create socket" << std::endl;
        return 1;
    }

    // Enable SCTP multicast reception
    int enable = 1;
    ret = setsockopt(sock_fd, IPPROTO_SCTP, SCTP_MULTICAST_ENABLE, &enable, sizeof(enable));
    if (ret < 0) 
    {
        std::cerr << "Failed to enable SCTP multicast" << std::endl;
        close(sock_fd);
        return 1;
    }

    // Set up multicast group address
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(MULTICAST_ADDR);
    addr.sin_port = htons(MULTICAST_PORT);

    // Join the multicast group
    memset(&mreq, 0, sizeof(mreq));
    mreq.imr_multiaddr.s_addr = inet_addr(MULTICAST_ADDR);
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    ret = setsockopt(sock_fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));
    if (ret < 0) 
    {
        std::cerr << "Failed to join multicast group" << std::endl;
        close(sock_fd);
        return 1;
    }

    // Bind the socket to the multicast address and port
    ret = bind(sock_fd, (struct sockaddr*)&addr, sizeof(addr));
    if (ret < 0) 
    {
        std::cerr << "Failed to bind socket" << std::endl;
        close(sock_fd);
        return 1;
    }

    // Receive multicast data
    while (true) 
    {
        memset(buffer, 0, BUFFER_SIZE);
        ret = recv(sock_fd, buffer, BUFFER_SIZE, 0);
        if (ret > 0) 
        {
            std::cout << "Received: " << buffer << std::endl;
        } 
        else 
        {
            std::cerr << "Failed to receive data" << std::endl;
            break;
        }
    }

    // Leave the multicast group
    ret = setsockopt(sock_fd, IPPROTO_IP, IP_DROP_MEMBERSHIP, &mreq, sizeof(mreq));
    if (ret < 0) 
    {
        std::cerr << "Failed to leave multicast group" << std::endl;
    }

    close(sock_fd);
    return 0;
}
