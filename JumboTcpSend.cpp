
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFER_SIZE 65536

int main(int argc, char *argv[]) 
{
    if (argc != 3) 
    {
        std::cerr << "Usage: " << argv[0] << " <server_ip> <server_port>" << std::endl;
        return 1;
    }

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) 
    {
        std::cerr << "Failed to create socket" << std::endl;
        return 1;
    }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(std::stoi(argv[2]));
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);

    if (connect(sockfd, (sockaddr*)&server_addr, sizeof(server_addr)) == -1) 
    {
        std::cerr << "Failed to connect to server" << std::endl;
        close(sockfd);
        return 1;
    }

    char buffer[BUFFER_SIZE];
    memset(buffer, 'X', sizeof(buffer));

    ssize_t bytes_sent = send(sockfd, buffer, sizeof(buffer), 0);
    if (bytes_sent == -1) 
    {
        std::cerr << "Failed to send data" << std::endl;
    } 
    else 
    {
        std::cout << "Sent " << bytes_sent << " bytes" << std::endl;
    }

    close(sockfd);
    return 0;
}
