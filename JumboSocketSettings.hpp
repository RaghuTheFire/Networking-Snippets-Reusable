
#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

// Jumbo UDP socket settings
int create_jumbo_udp_socket() 
{
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) 
    {
        std::cerr << "Failed to create UDP socket" << std::endl;
        return -1;
    }

    int optval = 9216; // Jumbo frame size (9000 bytes payload + headers)
    if (setsockopt(sock, SOL_SOCKET, SO_RCVBUF, &optval, sizeof(optval)) < 0) 
    {
        std::cerr << "Failed to set receive buffer size" << std::endl;
        close(sock);
        return -1;
    }

    if (setsockopt(sock, SOL_SOCKET, SO_SNDBUF, &optval, sizeof(optval)) < 0) 
    {
        std::cerr << "Failed to set send buffer size" << std::endl;
        close(sock);
        return -1;
    }

    return sock;
}

// Jumbo TCP socket settings
int create_jumbo_tcp_socket() 
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) 
    {
        std::cerr << "Failed to create TCP socket" << std::endl;
        return -1;
    }

    int optval = 1; // Enable socket option
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) 
    {
        std::cerr << "Failed to set socket option SO_REUSEADDR" << std::endl;
        close(sock);
        return -1;
    }

    optval = 9216; // Jumbo frame size (9000 bytes payload + headers)
    if (setsockopt(sock, SOL_SOCKET, SO_RCVBUF, &optval, sizeof(optval)) < 0) 
    {
        std::cerr << "Failed to set receive buffer size" << std::endl;
        close(sock);
        return -1;
    }

    if (setsockopt(sock, SOL_SOCKET, SO_SNDBUF, &optval, sizeof(optval)) < 0) 
    {
        std::cerr << "Failed to set send buffer size" << std::endl;
        close(sock);
        return -1;
    }

    return sock;
}
