
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFER_SIZE 65536 // Jumbo packet size (64KB)

int main() 
{
    int sockfd, newsockfd;
    sockaddr_in server_addr, client_addr;
    socklen_t client_len;
    char buffer[BUFFER_SIZE];

    // Create a TCP socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
    {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    // Set up server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080); // Change the port number as needed

    // Bind the socket to the server address
    if (bind(sockfd, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) 
    {
        std::cerr << "Error binding socket" << std::endl;
        return 1;
    }

    // Listen for incoming connections
    listen(sockfd, 5);
    std::cout << "Server listening on port 8080" << std::endl;

    while (true) 
    {
        client_len = sizeof(client_addr);
        newsockfd = accept(sockfd, (sockaddr*)&client_addr, &client_len);
        if (newsockfd < 0) 
        {
            std::cerr << "Error accepting connection" << std::endl;
            continue;
        }

        std::cout << "New connection accepted" << std::endl;

        // Receive jumbo packets
        int bytes_received;
        while ((bytes_received = recv(newsockfd, buffer, BUFFER_SIZE, 0)) > 0) 
        {
            // Process the received data
            std::cout << "Received " << bytes_received << " bytes" << std::endl;
            // Add your code here to handle the received data
        }

        if (bytes_received < 0) 
        {
            std::cerr << "Error receiving data" << std::endl;
        }

        // Close the connection
        close(newsockfd);
    }

    // Close the server socket
    close(sockfd);

    return 0;
}
