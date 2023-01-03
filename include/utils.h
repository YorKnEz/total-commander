#ifndef UTILS_H
#define UTILS_H

#include <string>

using namespace std;

int str2int(string numberString);

string compressSize(uintmax_t sizeToCompress);

string uint2str(uintmax_t a);
string int2str(int a);

string toLower(string s);

string formatDate(string date);

#endif
