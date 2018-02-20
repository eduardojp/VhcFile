#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <stdint.h>

#include "buffer.h"
#include "database.h"

using namespace std;

int BUFFER_SIZE = 100000;

int main(int argc, char** argv) {
    
    //Database* database = new Database("192.168.100.10", 5432, "vhc", "postgres", "postgres");
    Database* database = new Database("192.168.100.125", 5432, "vhcservice", "vhcservice", "vhcservice");
    database->loadHashes("'AEE'", "", "", 30);
    exit(0);
    
    //fseek(f, 0, SEEK_END);
    //long fsize = ftell(f);
    //fseek(f, 0, SEEK_SET);  //same as rewind(f);

    FILE* f = 0;
    Buffer* buffer = new Buffer(BUFFER_SIZE);
    size_t nBytes;
    
    const char* ext_ref = "AEE";
    const char* profile = "30 0,0,0,0 32x32 1";
    uint64_t time_ref = 1489901371000L;
    
    uint8_t prefix0 = (8469206151714042888L >> 56) & 0xFF;
    uint16_t nVHCs0 = 1;
        uint32_t hash_time0 = 26791;    int64_t vhc0 = 8469206151714042888L;

    uint8_t prefix1 = (-3948810815115837131L >> 56) & 0xFF;
    uint16_t nVHCs1 = 2;
        uint32_t hash_time1 = 27791;    int64_t vhc1 = -3948810815115837131L;
        uint32_t hash_time2 = 28791;    int64_t vhc2 = -3948810815115837131L;
    //27791 -3948810815115837131
    //28791 -3948810815115837131
    
    uint8_t prefix2 = (-4020866476502301389L >> 56) & 0xFF;
    uint16_t nVHCs2 = 1;
        uint32_t hash_time3 = 29791;    int64_t vhc3 = -4020866476502301389L;
    //29791 -4020866476502301389

    //----------------------------

    /* Write your buffer to disk. */
//    f = fopen("test.vhc", "wb");
//
//    if(f) {
//        buffer->storeString(ext_ref);
//        buffer->storeString(profile);
//        buffer->store64(time_ref);
//            buffer->store8(prefix0);
//            buffer->store16(nVHCs0);
//                buffer->store32(hash_time0);  buffer->storePrefixed64(vhc0);
//            buffer->store8(prefix1);
//            buffer->store16(nVHCs1);
//                buffer->store32(hash_time1);  buffer->storePrefixed64(vhc1);
//                buffer->store32(hash_time2);  buffer->storePrefixed64(vhc2);
//            buffer->store8(prefix2);
//            buffer->store16(nVHCs2);
//                buffer->store32(hash_time3);  buffer->storePrefixed64(vhc3);
//
//        printf("buffer->size: %ld\n", buffer->size);
//        fwrite(buffer->buffer, 1, buffer->size, f);
//        puts("Wrote to file!");
//        
//        fclose(f);
//    }
//    else{
//        puts("Something wrong writing to File.");
//    }
//    exit(0);

    //----------------------------
    
    f = fopen("1847.vhc", "rb");
    
    if(f) {
        while(nBytes = fread(buffer->buffer, 1, BUFFER_SIZE, f)) {
            printf("nBytes read: %ld\n", nBytes);
            
            char* s1 = buffer->getString();
            char* s2 = buffer->getString();
            printf("(%s)-(%s)\n", s1, s2);
            
            

            int i;
            for(i = 0; i < 25; i++) {
                printf("\n%d:", i);
                printf("%c/%d, ", buffer->buffer[i], buffer->buffer[i]);
            }
            printf("\n");
            
            exit(0);
            
            for(i = 0; i < nBytes; i++) {
                printf("%d:\n", i);
                printf("%d, ", buffer->buffer[i]);
            }
            printf("\n");
        }

        fclose(f);
    }
    else{
        puts("Something wrong writing to File.");
    }
    
    return 0;
}