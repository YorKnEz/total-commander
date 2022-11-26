#ifndef UTILS_H
#define UTILS_H
#include "dllist.h"
#include <string>

using namespace std;

struct Point {
  int x, y;
};

struct RGB {
  unsigned int r, g, b;
};

int str2int(string numberString);

string int2str(int a);

Filedata parseFileDataString(string filedataString);

void hexColorToRGB(RGB &color, unsigned int hex);

RGB getRGBFromHex(unsigned int hex);
#endif