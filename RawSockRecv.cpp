#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <net/if_arp.h>

int main() 
{
    int sockfd;
    struct sockaddr_ll saddr;
    char buffer[1024];

    // Create a raw socket
    if ((sockfd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ARP))) < 0) 
    {
        perror("socket");
        return 1;
    }

    // Bind the socket to the interface
    memset(&saddr, 0, sizeof(saddr));
    saddr.sll_family = AF_PACKET;
    saddr.sll_protocol = htons(ETH_P_ARP);
    if (bind(sockfd, (struct sockaddr *)&saddr, sizeof(saddr)) < 0) 
    {
        perror("bind");
        close(sockfd);
        return 1;
    }

    while (true) 
    {
        ssize_t n = recvfrom(sockfd, buffer, sizeof(buffer), 0, NULL, NULL);
        if (n < 0) 
        {
            perror("recvfrom");
            close(sockfd);
            return 1;
        }

        // Process the received ARP packet
        std::cout << "Received ARP packet of size " << n << " bytes" << std::endl;
    }

    close(sockfd);
    return 0;
}
