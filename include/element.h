#ifndef ELEMENT_H
#define ELEMENT_H
#include "dllist.h"
#include "utils.h"
#include "winbgim.h"

enum State { INACTIVE, HOVERED, CLICKED, DCLICKED };

struct Button {
  Point coords;
  int width, height;
  char *text;
  RGB textColor, borderColor, backgroundColor;
  State state, oldState;
};

struct Text {
  Point coords;
  int width, height;
  char *text;
  RGB textColor, borderColor, backgroundColor;
};

struct File {
  Point coords;
  int width, height;
  int filenameColumn, extColumn, sizeColumn, dateColumn;
  Filedata data;
  RGB textColor, borderColor, backgroundColor;
};

struct Input {
  Point coords;
  int width, height;
  char *placeholder;
  char *value;
};

union Element {
  Button button;
  Text text;
  File file;
  Input input;
};

// button functions
void drawButton(Button button);

bool isHovered(Button &button, Point mouse);

// text functions
void drawText(Text text);

// file functions
void drawFile(File file);

void isClicked(File file);

void isDoubleClicked(File file);

#endif