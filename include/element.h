#ifndef ELEMENT_H
#define ELEMENT_H
#include "dllist.h"
#include "utils.h"
#include "winbgim.h"
#include <string.h>
#include <string>

using namespace std;

enum State { INACTIVE, HOVERED, CLICKED, DCLICKED };

struct Button {
  Point coords;
  int width, height;
  string text;
  RGB textColor, borderColor, backgroundColor;
  State state, oldState;
};

struct Text {
  Point coords;
  int width, height;
  string text;
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
  string placeholder;
  string value;
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
Text createText(int x, int y, int width, int height, string textString,
                RGB textColor, RGB backgroundColor, RGB borderColor);

void drawText(Text text);

// file functions
void drawFile(File file);

void isClicked(File file);

void isDoubleClicked(File file);

#endif