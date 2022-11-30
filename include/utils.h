#ifndef UTILS_H
#define UTILS_H
#include "dllist.h"
#include <string>

using namespace std;

int str2int(string numberString);

string int2str(int a);

Filedata parseFileDataString(string filedataString);

#endif