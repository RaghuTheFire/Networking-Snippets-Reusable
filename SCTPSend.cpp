#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <netinet/sctp.h>
#include <unistd.h>

int main() 
{
    int sock_fd, ret;
    struct sockaddr_in server_addr;
    const char* message = "Hello, SCTP!";

    // Create a SCTP socket
    sock_fd = socket(AF_INET, SOCK_SEQPACKET, IPPROTO_SCTP);
    if (sock_fd < 0) 
    {
        std::cerr << "Failed to create socket" << std::endl;
        return 1;
    }

    // Set up the server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Replace with the server IP address
    server_addr.sin_port = htons(9000); // Replace with the server port

    // Connect to the server
    ret = connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (ret < 0) 
    {
        std::cerr << "Failed to connect to server" << std::endl;
        close(sock_fd);
        return 1;
    }

    // Send the message
    ret = sctp_sendmsg(sock_fd, (void*)message, strlen(message), NULL, 0, 0, 0, 0, 0, 0);
    if (ret < 0) 
    {
        std::cerr << "Failed to send message" << std::endl;
        close(sock_fd);
        return 1;
    }

    std::cout << "Message sent successfully" << std::endl;

    // Close the socket
    close(sock_fd);

    return 0;
}
