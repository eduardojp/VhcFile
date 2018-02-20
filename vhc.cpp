#include "vhc.h"

map<string, string*> VHC::stringMap;

VHC::VHC(uint64_t time, uint64_t vhc, string* extRef) {
    this->time = time;
    this->vhc = vhc;
    this->extRef = extRef;
}

string* VHC::getExtRef(string keyString) {
    string* extRef = stringMap[keyString];
    
    if(!extRef) {
        extRef = new string(keyString);
        stringMap[keyString] = extRef;
    }
    
    return extRef;
}