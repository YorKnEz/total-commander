#ifndef ELEMENT_H
#define ELEMENT_H
// #include "dllist.h"
// #include "utils.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

using namespace sf;
using namespace std;

#define MAX_STATES 4

enum MouseEventType { MOVE, RELEASE, CLICK, DCLICK };
enum State { INACTIVE, HOVERED, CLICKED, DCLICKED };

struct ButtonStateColors {
  Color primary, background;
};

struct Button {
  string fullText;
  ButtonStateColors buttonStateColors[MAX_STATES];
  RectangleShape background;
  Text text;
  State state, oldState;
};

// struct File {
//   Point coords;
//   int width, height;
//   int filenameColumn, extColumn, sizeColumn, dateColumn;
//   Filedata data;
//   RGB textColor, borderColor, backgroundColor;
// };

// struct Input {
//   Point coords;
//   int width, height;
//   string placeholder;
//   string value;
// };

// union Element {
//   Button button;
//   Text text;
//   File file;
//   Input input;
// };

// button functions
Button createButton(string text, Font &font, int x, int y, int width,
                    int height, ButtonStateColors buttonStateColors[MAX_STATES],
                    unsigned int borderThickness);

void drawButton(RenderWindow &window, Button button);

void updateButtonState(Button &button, Event event, MouseEventType type);

bool isHovered(Button &button, int mouseX, int mouseY);

// // text functions
// Text createText(int x, int y, int width, int height, string textString,
//                 RGB textColor, RGB backgroundColor, RGB borderColor);

// void drawText(Text text);

// // file functions
// void drawFile(File file);

// void isClicked(File file);

// void isDoubleClicked(File file);

#endif