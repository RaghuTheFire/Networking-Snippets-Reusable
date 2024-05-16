/**************************************************************************************************************************************************************
JUMBO frames, also known as JUMBO packets, are a type of data frames used in computer networking.They are larger than the standard maximum transmission unit (MTU) 
size of 1500 bytes, allowing for greater amounts of data to be transmitted in a single frame.
The most common jumbo frame is 9,000 bytes for the payload limit or MTU. While there is no specific standard, 9,000 bytes was chosen during the development of 
Gigabit Ethernet and was later adopted by most equipment vendors and agencies.
**************************************************************************************************************************************************************/
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define JUMBO_PACKET_SIZE 9000

int main() 
{
    int sockfd;
    struct sockaddr_in servaddr;
    char buffer[JUMBO_PACKET_SIZE];

    // Create socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) 
    {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    // Set socket options for jumbo packet
    int optval = JUMBO_PACKET_SIZE;
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &optval, sizeof(optval)) < 0) 
    {
        std::cerr << "Error setting socket option for receive buffer size" << std::endl;
        close(sockfd);
        return 1;
    }

    // Bind socket to local address
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(8000); // Replace with desired port number

    if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) 
    {
        std::cerr << "Error binding socket" << std::endl;
        close(sockfd);
        return 1;
    }

    // Receive jumbo packet
    socklen_t addrlen = sizeof(servaddr);
    ssize_t recvlen = recvfrom(sockfd, buffer, JUMBO_PACKET_SIZE, 0, (struct sockaddr*)&servaddr, &addrlen);
    if (recvlen < 0) 
    {
        std::cerr << "Error receiving packet" << std::endl;
        close(sockfd);
        return 1;
    }

    std::cout << "Received packet of size " << recvlen << " bytes" << std::endl;

    close(sockfd);
    return 0;
}
