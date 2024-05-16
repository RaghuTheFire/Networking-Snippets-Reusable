
#include <iostream>
#include <vector>
#include <pcap.h>

std::vector<std::string> getEthernetInterfaces() 
{
    std::vector<std::string> interfaces;
    pcap_if_t* alldevs;
    char errbuf[PCAP_ERRBUF_SIZE];

    // Get a list of all available interfaces
    if (pcap_findalldevs(&alldevs, errbuf) == -1) 
    {
        std::cerr << "Error getting interfaces: " << errbuf << std::endl;
        return interfaces;
    }

    // Iterate through the list and add Ethernet interfaces to the vector
    for (pcap_if_t* dev = alldevs; dev != nullptr; dev = dev->next) 
    {
        if (dev->description && strstr(dev->description, "Ethernet") != nullptr) 
        {
            interfaces.push_back(dev->name);
        }
    }

    // Free the interface list
    pcap_freealldevs(alldevs);

    return interfaces;
}

int main() 
{
    std::vector<std::string> interfaces = getEthernetInterfaces();

    std::cout << "Available Ethernet interfaces:" << std::endl;
    for (const std::string& iface : interfaces) 
    {
        std::cout << "- " << iface << std::endl;
    }

    return 0;
}
