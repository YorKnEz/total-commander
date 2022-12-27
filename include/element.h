#ifndef ELEMENT_H
#define ELEMENT_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

using namespace sf;
using namespace std;

enum MouseEventType { MOVE, RELEASE, CLICK, DCLICK };

struct StateColors {
  Color text, background, border;
};

// helpful functions
bool isHovered(FloatRect box, int mouseX, int mouseY);

RectangleShape createRect(int x, int y, int width, int height, Color fill,
                          Color outline = Color::Transparent,
                          int borderThickness = 0);

// text functions
Text createText(string textString, Font &font, int charSize, int x, int y,
                int width, Color text);

void updateText(Text &text, string newText, FloatRect bounds);

void drawText(RenderWindow &window, Text text);

#endif
