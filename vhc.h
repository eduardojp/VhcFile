#ifndef VHC_H
#define VHC_H

#include <map>
#include <string>
#include <stdint.h>

using namespace std;

class VHC {
    public:
        static map<string, string*> stringMap;
        static string* getExtRef(string keyString);
        
        uint64_t time;
        uint64_t vhc;
        string* extRef;
        
        VHC(uint64_t time, uint64_t vhc, string* extRef);
};

#endif