/**************************************************************************************************************************************************************
JUMBO frames, also known as JUMBO packets, are a type of data frames used in computer networking.They are larger than the standard maximum transmission unit (MTU) 
size of 1500 bytes, allowing for greater amounts of data to be transmitted in a single frame.
The most common jumbo frame is 9,000 bytes for the payload limit or MTU. While there is no specific standard, 9,000 bytes was chosen during the development of 
Gigabit Ethernet and was later adopted by most equipment vendors and agencies.
**************************************************************************************************************************************************************/

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define JUMBO_PACKET_SIZE 9000

int main() 
{
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) 
    {
        std::cerr << "Failed to create socket" << std::endl;
        return 1;
    }

    int optval = JUMBO_PACKET_SIZE;
    if (setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &optval, sizeof(optval)) < 0) 
    {
        std::cerr << "Failed to set send buffer size" << std::endl;
        close(sockfd);
        return 1;
    }

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(12345); // Replace with the desired port number
    servaddr.sin_addr.s_addr = inet_addr("192.168.1.100"); // Replace with the desired IP address

    char buffer[JUMBO_PACKET_SIZE];
    memset(buffer, 'X', JUMBO_PACKET_SIZE);

    ssize_t sent = sendto(sockfd, buffer, JUMBO_PACKET_SIZE, 0, (struct sockaddr*)&servaddr, sizeof(servaddr));
    if (sent < 0) 
    {
        std::cerr << "Failed to send data" << std::endl;
    } 
    else 
    {
        std::cout << "Sent " << sent << " bytes" << std::endl;
    }

    close(sockfd);
    return 0;
}
