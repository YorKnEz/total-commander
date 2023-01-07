#ifndef SCROLLBAR_H
#define SCROLLBAR_H

#include "button.h"
#include "element.h"

struct Scrollbar {
  Button up, down;
  int scrollableHeight, scrollOffset, oldMouseY;
  RectangleShape track, thumb;
  ButtonState state;
};

Scrollbar createScrollbar(Texture *upIcon, Texture *downIcon, Font &font,
                          int charSize, int x, int y, int width, int height,
                          int btnHeight, int scrollableHeight,
                          StateColors stateColors[B_MAX_STATES],
                          unsigned int borderThickness);

float getScrollbarRatio(Scrollbar scrollbar);

void updateScrollableHeight(Scrollbar &scrollbar, int scrollableHeight);

void updateScrollbar(Scrollbar &scrollbar, int scrollOffset);

void updateScrollbarOnDrag(Scrollbar &scrollbar, int &scrollOffset);

void updateScrollbarTheme(Scrollbar &scrollbar,
                          StateColors stateColors[B_MAX_STATES]);

void drawScrollbar(RenderWindow &window, Scrollbar scrollbar);

#endif
