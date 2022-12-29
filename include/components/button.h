#ifndef BUTTON_H
#define BUTTON_H

#include "element.h"

#define B_MAX_STATES 4

enum ButtonState { B_INACTIVE, B_HOVERED, B_CLICKED, B_DCLICKED };

struct Button {
  StateColors stateColors[B_MAX_STATES];
  RectangleShape background;
  string fullText;
  Text text;
  ButtonState state;
};

Button createButton(string text, Font &font, int charSize, int x, int y,
                    int width, int height,
                    StateColors stateColors[B_MAX_STATES],
                    unsigned int borderThickness = 0);

void drawButton(RenderWindow &window, Button button);

void updateButtonState(Button &button, Event event, MouseEventType type,
                       Vector2i &oldClick);

#endif
