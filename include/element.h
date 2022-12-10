#ifndef ELEMENT_H
#define ELEMENT_H
#include "dllist.h"
#include "utils.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

using namespace sf;
using namespace std;

#define B_MAX_STATES 4
#define I_MAX_STATES 3

enum MouseEventType { MOVE, RELEASE, CLICK, DCLICK };
enum ButtonState { B_INACTIVE, B_HOVERED, B_CLICKED, B_DCLICKED };
enum InputState { I_INACTIVE, I_HOVERED, I_ACTIVE };

struct ButtonStateColors {
  Color primary, background;
};

struct InputStateColors {
  Color primary, background;
};

struct TextBox {
  RectangleShape background;
  string fullText;
  Text text;
};

struct Button {
  ButtonStateColors buttonStateColors[B_MAX_STATES];
  RectangleShape background;
  string fullText;
  Text text;
  ButtonState state, oldState;
};

struct Filedata {
  string filename;
  string ext;
  string size;
  string date;
};

struct File {
  RectangleShape background;
  Filedata data;
  int filenameColumn, extColumn, sizeColumn, dateColumn;
  Text filename, ext, size, date;
};

struct Input {
  InputStateColors inputStateColors[I_MAX_STATES];
  RectangleShape background;
  string placeholder, value;
  Text displayText;
  InputState state;
  int cursorLocation;
  int startPosition; // used if the displayText is larger than the box
  int displayLength; // max length that can be displayed at once
};

bool isHovered(FloatRect box, int mouseX, int mouseY);
// textbox functions
TextBox createTextBox(string textString, Font &font, int charSize, int x, int y,
                      int width, int height, Color textColor,
                      Color backgroundColor, Color borderColor,
                      int borderThickness);

void drawTextBox(RenderWindow &window, TextBox textbox);

// button functions
Button createButton(string text, Font &font, int charSize, int x, int y,
                    int width, int height,
                    ButtonStateColors buttonStateColors[B_MAX_STATES],
                    unsigned int borderThickness);

void drawButton(RenderWindow &window, Button button);

void updateButtonState(Button &button, Event event, MouseEventType type,
                       FloatRect &clickBounds);

// // text functions
Text createText(string textString, Font &font, int charSize, int x, int y,
                int width, Color textColor);

void drawText(RenderWindow &window, Text text);

// file functions
File createFile(Filedata data, Font &font, int charSize, int x, int y,
                int width, int height, Color textColor);

void drawFile(RenderWindow &window, File file);

// input functions
Input createInput(string placeholder, string value, Font &font, int charSize,
                  int x, int y, int width, int height,
                  InputStateColors inputStateColors[I_MAX_STATES],
                  unsigned int borderThickness);

void updateInputState(Input &input, Event event, MouseEventType type,
                      Input *&activeInput);

void expandInput(Input &input);

void shrinkInput(Input &input);

void drawInput(RenderWindow &window, Input &input);

#endif
