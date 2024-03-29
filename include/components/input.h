#ifndef INPUT_H
#define INPUT_H

#include "element.h"

#define I_MAX_STATES 3

enum InputState { I_INACTIVE, I_HOVERED, I_ACTIVE };

struct Input {
  StateColors stateColors[I_MAX_STATES];
  RectangleShape background;
  string placeholder, value;
  Text displayText;
  InputState state;
  int cursorLocation;
  int startPosition; // used if the displayText is larger than the box
  int displayLength; // max length that can be displayed at once
};

// input functions
Input createInput(string placeholder, string value, Font &font, int charSize,
                  int x, int y, int width, int height,
                  StateColors stateColors[I_MAX_STATES],
                  unsigned int borderThickness = 0);

void updateInputState(Input &input, Event event, MouseEventType type,
                      Input *&activeInput);

void updateInputText(Input &input, string newValue, string newPlaceholder = "");

void updateInputTheme(Input &input, StateColors stateColors[I_MAX_STATES]);

void insertChar(Input *activeInput, char c);

void eraseChar(Input *activeInput);

void shrinkInput(Input &input);

void drawInput(RenderWindow &window, Input &input);

void moveCursor(Input *activeInput, int direction);

void drawCursor(RenderWindow &window, Input *activeInput);

#endif
