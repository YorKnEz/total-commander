#ifndef EXPLORER_H
#define EXPLORER_H
#include "element.h"
#include "filetree.h"
#include <string>

using namespace std;

struct Explorer {
  string path;
  list files;
  Element *elements;
};

#endif
