#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "element.h"

struct TextBox {
  RectangleShape background;
  string fullText;
  Text text;
};

TextBox createTextBox(string textString, Font &font, int charSize, int x, int y,
                      int width, int height, Color text = Color::Transparent,
                      Color bg = Color::Transparent,
                      Color border = Color::Transparent,
                      int borderThickness = 0);

void updateTextBoxTheme(TextBox &textbox, Color text, Color bg, Color border);

void drawTextBox(RenderWindow &window, TextBox textbox);

#endif
