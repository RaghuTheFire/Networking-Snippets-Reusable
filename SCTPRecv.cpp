
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() 
{
    int sockfd, n;
    struct sockaddr_in servaddr;
    char recvline[1024];

    // Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);
    if (sockfd < 0) 
    {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    // Set up the server address
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Replace with the server IP address
    servaddr.sin_port = htons(8000); // Replace with the server port number

    // Connect to the server
    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) 
    {
        std::cerr << "Error connecting to server" << std::endl;
        return 1;
    }

    // Receive data from the server
    while ((n = recv(sockfd, recvline, 1024, 0)) > 0) 
    {
        recvline[n] = '\0';
        std::cout << "Received: " << recvline << std::endl;
    }

    if (n < 0) 
    {
        std::cerr << "Error receiving data" << std::endl;
    }

    // Close the socket
    close(sockfd);

    return 0;
}
