#include <string.h>
#include "buffer.h"

Buffer::Buffer(int capacity) {
    this->position = 0;
    this->capacity = capacity;
    this->buffer = (uint8_t*) malloc(capacity * sizeof(uint8_t));
}

int Buffer::store8(uint8_t value) {
    if(position + 1 > capacity) {
        return -1;
    }
    
    buffer[position] = value;

    position += 1;
    
    return 0;
}

int Buffer::store16(uint16_t value) {
    if(position + 2 > capacity) {
        return -1;
    }
    
    buffer[position] = (value >> 8) & 0xFF;
    buffer[position+1] = value & 0xFF;
    
    position += 2;
    
    return 0;
}

int Buffer::store32(uint32_t value) {
    if(position + 4 > capacity) {
        return -1;
    }
    
    buffer[position] = (value >> 24) & 0xFF;
    buffer[position+1] = (value >> 16) & 0xFF;
    buffer[position+2] = (value >> 8) & 0xFF;
    buffer[position+3] = value & 0xFF;
    
    position += 4;
    
    return 0;
}

int Buffer::store64(uint64_t value) {
    if(position + 8 > capacity) {
        return -1;
    }
    
    buffer[position] = (value >> 56) & 0xFF;
    buffer[position+1] = (value >> 48) & 0xFF;
    buffer[position+2] = (value >> 40) & 0xFF;
    buffer[position+3] = (value >> 32) & 0xFF;
    buffer[position+4] = (value >> 24) & 0xFF;
    buffer[position+5] = (value >> 16) & 0xFF;
    buffer[position+6] = (value >> 8) & 0xFF;
    buffer[position+7] = value & 0xFF;
    
    position += 8;
    
    return 0;
}

int Buffer::storePrefixed64(uint64_t value) {
    if(position + 7 > capacity) {
        return -1;
    }
    
    //buffer[size] = (value >> 56) & 0xFF;
    buffer[position] = (value >> 48) & 0xFF;
    buffer[position+1] = (value >> 40) & 0xFF;
    buffer[position+2] = (value >> 32) & 0xFF;
    buffer[position+3] = (value >> 24) & 0xFF;
    buffer[position+4] = (value >> 16) & 0xFF;
    buffer[position+5] = (value >> 8) & 0xFF;
    buffer[position+6] = value & 0xFF;
    
    position += 7;
    
    return 0;
}

int Buffer::storeString(const char* value) {
    size_t stringSize = strlen(value);
    
    if(position + stringSize > capacity) {
        return -1;
    }
    
    int i;
    for(i = 0; i < stringSize; i++) {
        buffer[position+i] = value[i];
    }
    buffer[position+i] = 0;
    
    position += stringSize+1;
    
    return 0;
}

uint8_t Buffer::get8() {
    if(position + 1 > capacity) {
        return -1;
    }
    
    uint8_t value = buffer[position];

    position += 1;
    
    return value;
}

uint16_t Buffer::get16() {
    if(position + 2 > capacity) {
        return -1;
    }
    
    uint16_t value = 0;
    
    value |= buffer[position] << 8;
    value |= buffer[position+1];
    
    position += 2;
    
    return value;
}

uint32_t Buffer::get32() {
    if(position + 4 > capacity) {
        return -1;
    }
    
    uint32_t value = 0;
    
    value |= buffer[position] << 24;
    value |= buffer[position+1] << 16;
    value |= buffer[position+2] << 8;
    value |= buffer[position+3];
    
    position += 4;
    
    return value;
}

uint64_t Buffer::get64() {
        if(position + 8 > capacity) {
        return -1;
    }

    uint64_t value = 0;
    
    value |= (uint64_t) buffer[position] << 56;
    value |= (uint64_t) buffer[position+1] << 48;
    value |= (uint64_t) buffer[position+2] << 40;
    value |= (uint64_t) buffer[position+3] << 32;
    value |= (uint64_t) buffer[position+4] << 24;
    value |= (uint64_t) buffer[position+5] << 16;
    value |= (uint64_t) buffer[position+6] << 8;
    value |= (uint64_t) buffer[position+7];
    
    position += 8;
    
    return 0;
}

uint64_t Buffer::getPrefixed64() {
    if(position + 7 > capacity) {
        return -1;
    }
    
    uint64_t value = 0;
    
    //buffer[size] = (value >> 56) & 0xFF;
    value |= (uint64_t) buffer[position] << 48;
    value |= (uint64_t) buffer[position+1] << 40;
    value |= (uint64_t) buffer[position+2] << 32;
    value |= (uint64_t) buffer[position+3] << 24;
    value |= (uint64_t) buffer[position+4] << 16;
    value |= (uint64_t) buffer[position+5] << 8;
    value |= (uint64_t) buffer[position+6];
    
    position += 7;
    
    return value;
}

char* Buffer::getString() {
    int i, j;
    
    for(i = position; i < capacity; i++) {
        printf("%d: %d\n", buffer[i], buffer[i]);
        
        if(buffer[i] == 0) {
            break;
        }
    }
    
    int size = i-position;
    char* value = (char*) malloc((size+1) * sizeof(char));
    
    for(i = position, j = 0; j < size; j++, i++) {
        value[j] = buffer[i];
    }
    value[j] = 0;
    
    position += size+1;
    
    return value;
}