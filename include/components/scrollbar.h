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

Scrollbar createScrollbar(Font &font, int charSize, int x, int y, int width,
                          int height, int scrollableHeight, int btnHeight,
                          StateColors stateColors[B_MAX_STATES],
                          unsigned int borderThickness = 0);

float getScrollbarRatio(Scrollbar scrollbar);

void updateScrollableHeight(Scrollbar &scrollbar, int scrollableHeight);

void updateScrollbar(Scrollbar &scrollbar, int scrollOffset);

void updateScrollbarOnDrag(Scrollbar &scrollbar, int &scrollOffset);

void updateScrollbarTheme(Scrollbar &scrollbar,
                          StateColors stateColors[B_MAX_STATES]);

void drawScrollbar(RenderWindow &window, Scrollbar scrollbar);

#endif
