#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFFER_SIZE 65507 // Maximum UDP packet size

int main() 
{
    int sockfd;
    struct sockaddr_in servaddr;
    char buffer[BUFFER_SIZE];

    // Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
    {
        std::cerr << "Socket creation failed" << std::endl;
        return 1;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Loopback address
    servaddr.sin_port = htons(8000); // Server port

    // Test case 1: Send maximum size UDP packet
    memset(buffer, 'A', BUFFER_SIZE);
    if (sendto(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) 
    {
        std::cerr << "Sendto failed" << std::endl;
        return 1;
    }
    std::cout << "Sent maximum size UDP packet" << std::endl;

    // Test case 2: Send UDP packet larger than maximum size (should fail)
    memset(buffer, 'B', BUFFER_SIZE + 1);
    if (sendto(sockfd, buffer, BUFFER_SIZE + 1, 0, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) 
    {
        std::cerr << "Sendto failed (expected for packet larger than maximum size)" << std::endl;
    } 
    else 
    {
        std::cerr << "Sendto should have failed for packet larger than maximum size" << std::endl;
        return 1;
    }

    close(sockfd);
    return 0;
}
