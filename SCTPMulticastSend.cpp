
#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <netinet/sctp.h>
#include <unistd.h>

int main() 
{
    int sock_fd, ret;
    struct sockaddr_in addr;
    const char* multicast_addr = "230.0.0.1";
    const int multicast_port = 5000;
    const char* message = "Hello, SCTP Multicast!";

    // Create SCTP socket
    sock_fd = socket(AF_INET, SOCK_SEQPACKET, IPPROTO_SCTP);
    if (sock_fd < 0) 
    {
        std::cerr << "Failed to create socket" << std::endl;
        return 1;
    }

    // Set up multicast address
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(multicast_addr);
    addr.sin_port = htons(multicast_port);

    // Enable SCTP multicast
    int option = 1;
    ret = setsockopt(sock_fd, IPPROTO_SCTP, SCTP_MULTICAST_ENABLE, &option, sizeof(option));
    if (ret < 0) 
    {
        std::cerr << "Failed to enable SCTP multicast" << std::endl;
        close(sock_fd);
        return 1;
    }

    while (true) 
    {
        // Send multicast message
        ret = sendto(sock_fd, message, strlen(message), 0, (struct sockaddr*)&addr, sizeof(addr));
        if (ret < 0) 
        {
            std::cerr << "Failed to send multicast message" << std::endl;
            close(sock_fd);
            return 1;
        }

        std::cout << "Sent multicast message: " << message << std::endl;
        sleep(1); // Wait for 1 second before sending the next message
    }

    close(sock_fd);
    return 0;
}
