#include <iostream>
#include <pcap.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <net/if_arp.h>
#include <unordered_map>

using namespace std;

// Function to handle ARP packets
void handleARPPacket(const u_char * packet, unordered_map < uint32_t, uint8_t * > & arpCache) 
{
  const struct ether_header * etherHeader = (struct ether_header * ) packet;
  const struct arp_header * arpHeader = (struct arp_header * )(packet + sizeof(struct ether_header));

  // Check if the packet is an ARP reply
  if (ntohs(arpHeader -> ar_op) == ARPOP_REPLY) 
  {
    uint32_t ipAddress = arpHeader -> ar_sip;
    uint8_t * macAddress = arpHeader -> ar_sha;

    // Check if the IP-MAC mapping already exists in the cache
    auto it = arpCache.find(ipAddress);
    if (it != arpCache.end()) 
    {
      // If the MAC address is different, it's a potential ARP spoofing attack
      if (memcmp(it -> second, macAddress, ETHER_ADDR_LEN) != 0) 
      {
        cout << "Potential ARP spoofing attack detected!" << endl;
        cout << "IP Address: " << inet_ntoa( * (in_addr * ) & ipAddress) << endl;
        cout << "Cached MAC: ";
        for (int i = 0; i < ETHER_ADDR_LEN; i++) 
        
          printf("%02X", it -> second[i]);
          if (i != ETHER_ADDR_LEN - 1) cout << ":";
        }
        cout << endl;
        cout << "Received MAC: ";
        for (int i = 0; i < ETHER_ADDR_LEN; i++) 
        {
          printf("%02X", macAddress[i]);
          if (i != ETHER_ADDR_LEN - 1) cout << ":";
        }
        cout << endl << endl;
      }
    } 
    else 
    {
      // Add the new IP-MAC mapping to the cache
      uint8_t * newMacAddress = new uint8_t[ETHER_ADDR_LEN];
      memcpy(newMacAddress, macAddress, ETHER_ADDR_LEN);
      arpCache[ipAddress] = newMacAddress;
    }
  }
}

// Callback function for pcap_loop
void packetHandler(u_char * args,
  const struct pcap_pkthdr * header,
    const u_char * packet) 
    {
  unordered_map < uint32_t, uint8_t * > * arpCache = (unordered_map < uint32_t, uint8_t * > * ) args;
  handleARPPacket(packet, * arpCache);
}

int main() 
{
  char * deviceName = pcap_lookupdev(nullptr);
  if (deviceName == nullptr) 
  {
    cerr << "Error: Unable to find a network device" << endl;
    return 1;
  }

  pcap_t * handle = pcap_open_live(deviceName, BUFSIZ, 1, 1000, nullptr);
  if (handle == nullptr) 
  {
    cerr << "Error: Unable to open the network device" << endl;
    return 1;
  }

  unordered_map < uint32_t, uint8_t * > arpCache;
  pcap_loop(handle, -1, packetHandler, (u_char * ) & arpCache);
  pcap_close(handle);

  // Clean up the ARP cache
  for (auto & entry: arpCache) 
  {
    delete[] entry.second;
  }

  return 0;
}
