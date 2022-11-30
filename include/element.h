#ifndef ELEMENT_H
#define ELEMENT_H
#include "dllist.h"
#include "utils.h"
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

struct File {
  Filedata data;
  RectangleShape background;
  Text filename, ext, size, date;
};

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
Text createText(string textString, Font &font, int charSize, int x, int y,
                int width, Color textColor);

void drawText(RenderWindow &window, Text text);

// file functions
File createFile(Filedata data, Font &font, int charSize, int x, int y,
                int width, int height, Color textColor);

void drawFile(RenderWindow &window, File file);

bool isHovered(File &file, int mouseX, int mouseY);

#endif