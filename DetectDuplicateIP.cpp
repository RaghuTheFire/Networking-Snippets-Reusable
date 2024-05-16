#include <iostream>
#include <unistd.h>
#include <string>
#include <map>
#include <vector>

std::string exec(const char * cmd) 
{
  std::array < char, 128 > buffer;
  std::string result;
  std::unique_ptr < FILE, decltype( & pclose) > pipe(popen(cmd, "r"), pclose);
  if (!pipe) 
  {
    throw std::runtime_error("popen() failed!");
  }
  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) 
  {
    result += buffer.data();
  }
  return result;
}

int main() 
{
  std::string arpOutput = exec("arp -a");
  std::map < std::string, std::vector < std::string >> ipToMacMap;

  std::istringstream iss(arpOutput);
  std::string line;
  while (std::getline(iss, line)) 
  {
    std::istringstream lineStream(line);
    std::string ipAddress, macAddress;
    lineStream >> ipAddress >> macAddress;
    if (!ipAddress.empty() && !macAddress.empty()) 
    {
      ipToMacMap[macAddress].push_back(ipAddress);
    }
  }

  for (const auto & [mac, ipList]: ipToMacMap) 
  {
    if (ipList.size() > 1) 
    {
      std::cout << "Duplicate IP addresses found for MAC address " << mac << ":\n";
      for (const auto & ip: ipList) 
      {
        std::cout << "  " << ip << "\n";
      }
      std::cout << "\n";
    }
  }

  return 0;
}
