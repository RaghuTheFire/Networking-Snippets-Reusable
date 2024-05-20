
#include <iostream>
#include <string>
#include <sstream>
#include <bitset>

std::string ipv4ToIPv6(const std::string& ipv4) 
{
    std::stringstream ss(ipv4);
    std::string token;
    std::bitset<32> bits;
    int i = 0;
    while (std::getline(ss, token, '.')) 
    {
        int value = std::stoi(token);
        bits |= (std::bitset<8>(value.to_ulong()) << (24 - i * 8));
        i++;
    }

    std::string ipv6;
    for (int i = 0; i < 8; i++) 
    {
        std::bitset<16> word(bits.to_string().substr(i * 4, 4));
        ipv6 += word.to_string(std::string::traits_type::value_type(16), '0');
        if (i < 7) 
        {
            ipv6 += ":";
        }
    }

    return ipv6;
}

int main() 
{
    std::string ipv4 = "192.168.0.1";
    std::string ipv6 = ipv4ToIPv6(ipv4);
    std::cout << "IPv6 address: " << ipv6 << std::endl;
    return 0;
}
