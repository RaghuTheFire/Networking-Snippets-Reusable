
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

int create_jumbo_sctp_socket() 
{
    int sock_fd;
    struct sockaddr_in servaddr;
    struct sctp_initmsg initmsg;
    int val;

    // Create an SCTP socket
    sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);
    if (sock_fd < 0) 
    {
        perror("socket");
        return -1;
    }

    // Set SCTP socket options for Jumbo frames
    memset(&initmsg, 0, sizeof(initmsg));
    initmsg.sinit_num_ostreams = 1;
    initmsg.sinit_max_instreams = 1;
    initmsg.sinit_max_attempts = 4;
    val = 65535; // Set Jumbo frame size
    if (setsockopt(sock_fd, IPPROTO_SCTP, SCTP_INITMSG, &initmsg, sizeof(initmsg)) < 0) 
    {
        perror("setsockopt SCTP_INITMSG");
        close(sock_fd);
        return -1;
    }
    if (setsockopt(sock_fd, IPPROTO_SCTP, SCTP_MAXSEG, &val, sizeof(val)) < 0) 
    {
        perror("setsockopt SCTP_MAXSEG");
        close(sock_fd);
        return -1;
    }

   return sock_fd;
}
