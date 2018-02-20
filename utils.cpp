#include "utils.h"
#include "buffer.h"
#include "vhc.h"

void saveVHCs(string fileName, string ext_ref, string profile, uint64_t time_ref, list<VHC*>* vhcList) {
    if(vhcList->empty()) {
        return;
    }
    
    FILE* f = fopen(fileName.c_str(), "wb");
    Buffer* buffer = new Buffer(100000);

    if(f) {
        buffer->storeString(ext_ref.c_str());
        buffer->storeString(profile.c_str());
        buffer->store64(time_ref);
        
        list<VHC*> prefixList;
        
        uint8_t prevPrefix = ((vhcList->front()->vhc) >> 56) & 0xFF;

        while(!vhcList->empty()) {
            VHC* vhc = vhcList->front();
            vhcList->pop_front();
            uint8_t prefix = ((vhc->vhc) >> 56) & 0xFF;
            
            //flush
            if(prefix != prevPrefix) {
                //printf("%d %d\n", prevPrefix, prefixList.size());
                
                buffer->store8(prevPrefix);
                buffer->store16((uint16_t) prefixList.size());
                
                for(auto it = prefixList.begin(); it != prefixList.end(); it++) {
                    VHC* v = (*it);
                    buffer->store32(v->time);
                    buffer->storePrefixed64(v->vhc);
                }
                
                prefixList.clear();
            }
            
            prevPrefix = prefix;
            prefixList.push_back(vhc);
        }
        //flush
        {
            //printf("%d %d\n", prevPrefix, prefixList.size());
            buffer->store8(prevPrefix);
            buffer->store16((uint16_t) prefixList.size());

            for(auto it = prefixList.begin(); it != prefixList.end(); it++) {
                VHC* v = (*it);
                buffer->store32(v->time);
                buffer->storePrefixed64(v->vhc);
            }
        }
        
//        int i;
//        for(i = 0; i < 100; i++) {
//            printf("%d: %d\n", i, buffer->buffer[i]);
//        }

        printf("buffer->size: %ld\n", buffer->position);
        fwrite(buffer->buffer, 1, buffer->position, f);
        puts("Wrote to file!");
        
        fclose(f);
    }
    else{
        puts("Something wrong writing to File.");
    }
}

list<VHC*>* loadVHCs(string fileName) {
    list<VHC*>* vhcList = new list<VHC*>();
    
    FILE* f = fopen(fileName.c_str(), "rb");
    size_t nBytes;
    Buffer* buffer = new Buffer(100000);

    if(f) {
        while(nBytes = fread(buffer->buffer, 1, BUFFER_SIZE, f)) {
            
        }
        
        string ext_ref = buffer->getString();
        string profile = buffer->getString(.c_str());
        uint64_t time_ref = buffer->store64();
        
        string* ext_ref_p = VHC.getExtRef(ext_ref);
        {
            uint64_t prefix = buffer->get8() << 56;
            uint16_t nVHCs = buffer->get16();

            int i;
            for(i = 0; i < nVHCs; i++) {
                uint64_t absolute_time = time_ref + buffer->get32();
                uint64_t vhc = prefix | buffer->getPrefixed64();
            }
        }

        printf("buffer->size: %ld\n", buffer->position);
        fwrite(buffer->buffer, 1, buffer->position, f);
        puts("Wrote to file!");
        
        fclose(f);
    }
    else{
        puts("Something wrong writing to File.");
    }
    
    return vhcList;
}

void replaceAll(string& str, const string& from, const string& to) {
    if(from.empty())
        return;
    
    size_t start_pos = 0;
    
    while((start_pos = str.find(from, start_pos)) != string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}