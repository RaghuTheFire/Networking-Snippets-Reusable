
#include <iostream>
#include <vector>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

struct SocketStats 
{
    int sockfd;
    std::string remoteIP;
    int remotePort;
    int bytesReceived;
    int bytesSent;
};

std::vector<SocketStats> getSocketStats() 
{
    std::vector<SocketStats> stats;
    int maxFd = getdtablesize();

    for (int fd = 0; fd < maxFd; ++fd) 
    {
        sockaddr_in remoteAddr;
        socklen_t remoteAddrLen = sizeof(remoteAddr);
        int bytesReceived = 0, bytesSent = 0;

        if (getpeername(fd, (sockaddr*)&remoteAddr, &remoteAddrLen) == 0) 
        {
            getsockopt(fd, SOL_SOCKET, SO_RCVBUF, &bytesReceived, &remoteAddrLen);
            getsockopt(fd, SOL_SOCKET, SO_SNDBUF, &bytesSent, &remoteAddrLen);

            SocketStats stat;
            stat.sockfd = fd;
            stat.remoteIP = inet_ntoa(remoteAddr.sin_addr);
            stat.remotePort = ntohs(remoteAddr.sin_port);
            stat.bytesReceived = bytesReceived;
            stat.bytesSent = bytesSent;
            stats.push_back(stat);
        }
    }

    return stats;
}

int main() 
{
    std::vector<SocketStats> stats = getSocketStats();

    for (const auto& stat : stats) 
    {
        std::cout << "Socket: " << stat.sockfd << std::endl;
        std::cout << "Remote IP: " << stat.remoteIP << std::endl;
        std::cout << "Remote Port: " << stat.remotePort << std::endl;
        std::cout << "Bytes Received: " << stat.bytesReceived << std::endl;
        std::cout << "Bytes Sent: " << stat.bytesSent << std::endl;
        std::cout << std::endl;
    }

    return 0;
}
