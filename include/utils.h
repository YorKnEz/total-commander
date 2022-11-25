#ifndef UTILS_H
#define UTILS_H
#include "dllist.h"
#include <string.h>

struct Point {
  int x, y;
};

struct RGB {
  unsigned int r, g, b;
};

int str2int(char *stringNumber);

char *int2str(int a);

Filedata parseFileDataString(char *filedataString);

void hexColorToRGB(RGB &color, unsigned int hex);

RGB getRGBFromHex(unsigned int hex);
#endif