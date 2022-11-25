#ifndef EXPLORER_H
#define EXPLORER_H
#include "element.h"
#include "filetree.h"

struct Explorer {
  char *path;
  list files;
  Element *elements;
};
#endif