
#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFFER_SIZE 65507 // Maximum UDP packet size

int main() {
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len;
    char buffer[BUFFER_SIZE];

    // Create a UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        std::cerr << "Failed to create socket" << std::endl;
        return 1;
    }

    // Set up server address
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(8888); // Change the port number as needed

    // Bind the socket to the server address
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        std::cerr << "Failed to bind socket" << std::endl;
        return 1;
    }

    std::cout << "Server is listening on port 8888..." << std::endl;

    while (true) {
        len = sizeof(cliaddr);
        memset(buffer, 0, BUFFER_SIZE);

        // Receive a UDP packet
        ssize_t n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&cliaddr, &len);
        if (n < 0) {
            std::cerr << "Failed to receive packet" << std::endl;
            continue;
        }

        std::cout << "Received packet of size " << n << " bytes" << std::endl;

        // Process the received packet
        // ...
    }

    close(sockfd);
    return 0;
}
