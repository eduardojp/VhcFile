#ifndef BUFFER_H
#define BUFFER_H

#include <stdlib.h>
#include <stdint.h>
#include <string>

using namespace std;

class Buffer {
    public:
        uint8_t* buffer;
        int capacity;
        size_t position;

        Buffer(int capacity);
        ~Buffer();

        int store8(uint8_t value);
        int store16(uint16_t value);
        int store32(uint32_t value);
        int store64(uint64_t value);
        int storePrefixed64(uint64_t value);
        int storeString(const char* value);
        
        uint8_t get8();
        uint16_t get16();
        uint32_t get32();
        uint64_t get64();
        uint64_t getPrefixed64();
        char* getString();
};

#endif