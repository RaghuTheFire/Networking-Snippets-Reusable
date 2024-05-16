
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>

int main() 
{
    int sockfd;
    struct sockaddr_ll addr;
    char buffer[ETH_FRAME_LEN];
    int bytes_sent;

    // Create a raw socket
    sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
    if (sockfd == -1) 
    {
        perror("socket");
        return 1;
    }

    // Set the interface index
    memset(&addr, 0, sizeof(addr));
    addr.sll_family = AF_PACKET;
    addr.sll_ifindex = if_nametoindex("eth0"); // Replace with your interface name
    if (addr.sll_ifindex == 0) 
    {
        perror("if_nametoindex");
        close(sockfd);
        return 1;
    }

    // Fill the buffer with your data
    memset(buffer, 0, ETH_FRAME_LEN);
    // ... Fill the buffer with your data ...

    while (true) 
    {
        // Send the data
        bytes_sent = sendto(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&addr, sizeof(addr));
        if (bytes_sent == -1) 
        {
            perror("sendto");
            close(sockfd);
            return 1;
        }

        // Sleep for a short duration (adjust as needed)
        usleep(100000); // 100 milliseconds
    }

    close(sockfd);
    return 0;
}
