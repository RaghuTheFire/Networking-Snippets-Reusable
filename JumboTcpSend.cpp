
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cerrno>

#define BUFFER_SIZE 65536

int main() 
{
    // Create a TCP socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
    {
        std::cerr << "Failed to create socket: " << strerror(errno) << std::endl;
        return 1;
    }

    // Set up the server address
    struct sockaddr_in server_addr;
    std::memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Replace with the server IP address
    server_addr.sin_port = htons(8000); // Replace with the server port

    // Connect to the server
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) 
    {
        std::cerr << "Failed to connect to server: " << strerror(errno) << std::endl;
        close(sockfd);
        return 1;
    }

    // Test case 1: Send a jumbo packet
    char buffer[BUFFER_SIZE];
    std::memset(buffer, 'A', BUFFER_SIZE - 1);
    buffer[BUFFER_SIZE - 1] = '\0';

    ssize_t bytes_sent = send(sockfd, buffer, BUFFER_SIZE, 0);
    if (bytes_sent < 0) 
    {
        std::cerr << "Failed to send data: " << strerror(errno) << std::endl;
        close(sockfd);
        return 1;
    }

    std::cout << "Sent " << bytes_sent << " bytes" << std::endl;

    // Test case 2: Send a jumbo packet with partial send
    std::memset(buffer, 'B', BUFFER_SIZE - 1);
    buffer[BUFFER_SIZE - 1] = '\0';

    ssize_t total_sent = 0;
    while (total_sent < BUFFER_SIZE) 
    {
        bytes_sent = send(sockfd, buffer + total_sent, BUFFER_SIZE - total_sent, 0);
        if (bytes_sent < 0) 
        {
            std::cerr << "Failed to send data: " << strerror(errno) << std::endl;
            close(sockfd);
            return 1;
        }
        total_sent += bytes_sent;
    }

    std::cout << "Sent " << total_sent << " bytes" << std::endl;

    // Close the socket
    close(sockfd);

    return 0;
}
