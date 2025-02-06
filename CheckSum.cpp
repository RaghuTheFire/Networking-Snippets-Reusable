#include <iostream>
#include <cstdint>

uint8_t byteChecksum(const uint8_t* data, size_t length) 
{
    uint8_t checksum = 0;
    for (size_t i = 0; i < length; ++i) 
    {
        checksum ^= data[i];
    }
    return checksum;
}

uint16_t wordChecksum(const uint16_t* data, size_t length) 
{
    uint16_t checksum = 0;
    for (size_t i = 0; i < length; ++i) 
    {
        checksum ^= data[i];
    }
    return checksum;
}

int main()
{
    uint8_t byteData[] = {1, 2, 3, 4, 5};
    uint16_t wordData[] = {1, 2, 3, 4, 5};

    uint8_t bChecksum = byteChecksum(byteData, sizeof(byteData) / sizeof(byteData[0]));
    uint16_t wChecksum = wordChecksum(wordData, sizeof(wordData) / sizeof(wordData[0]));

    std::cout << "Byte Checksum: " << static_cast<int>(bChecksum) << std::endl;
    std::cout << "Word Checksum: " << wChecksum << std::endl;

    return 0;
}
