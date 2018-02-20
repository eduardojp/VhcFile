#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <list>

#include "vhc.h"

using namespace std;

void saveVHCs(string fileName, string ext_ref, string profile, uint64_t time_ref, list<VHC*>* vhcList);
list<VHC*>* loadVHCs(string fileName);
void replaceAll(string& str, const string& from, const string& to);

#endif