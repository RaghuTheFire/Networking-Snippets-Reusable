
#include <iostream>
#include <arpa/inet.h>

bool isMulticastAddress(const char* ipAddress) 
{
    unsigned long addr = inet_addr(ipAddress);
    unsigned long multicastRange = 0xE0000000; // 224.0.0.0 to 239.255.255.255

    return (addr & multicastRange) == multicastRange;
}

int main() 
{
    const char* ipAddress1 = "224.0.0.1";
    const char* ipAddress2 = "192.168.1.1";

    std::cout << "Is " << ipAddress1 << " a multicast address? " << std::boolalpha << isMulticastAddress(ipAddress1) << std::endl;
    std::cout << "Is " << ipAddress2 << " a multicast address? " << std::boolalpha << isMulticastAddress(ipAddress2) << std::endl;

    return 0;
}
