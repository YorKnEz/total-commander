#include "element.h"

// checks if a certain box (button, text, file, etc.) is hoeverd by the mouse
bool isHovered(FloatRect box, int mouseX, int mouseY) {
  if ((box.left <= mouseX && mouseX <= box.left + box.width) &&
      (box.top <= mouseY && mouseY <= box.top + box.height)) {
    return true;
  }

  return false;
}

RectangleShape createRect(int x, int y, int width, int height, Color fill,
                          Color outline, int borderThickness) {
  RectangleShape r;

  r.setPosition(x, y);
  r.setSize(Vector2f(width, height));
  r.setFillColor(fill);
  r.setOutlineColor(outline);
  r.setOutlineThickness(borderThickness);

  return r;
}

Text createText(string textString, Font &font, int charSize, int x, int y,
                int width, Color text) {
  Text textObj;
  textObj.setFont(font);
  textObj.setPosition(x, y);
  textObj.setCharacterSize(charSize);
  textObj.setStyle(Text::Regular);
  textObj.setString(textString);
  textObj.setFillColor(text);

  // shrink the text that is shown on the screen to avoid overflow
  if (textObj.getGlobalBounds().width > width) {
    textString.append("..");
    textObj.setString(textString);
  }

  while (textObj.getGlobalBounds().width > width) {
    textString.erase(textString.size() - 3, 1);
    textObj.setString(textString);
  }

  return textObj;
}

void updateText(Text &text, string newText, FloatRect bounds) {
  int oldWidth = text.getGlobalBounds().width;
  text.setString(newText);

  // shrink the text that is shown on the screen to avoid overflow
  if (text.getGlobalBounds().width > bounds.width) {
    newText.append("..");
    text.setString(newText);
  }

  while (text.getGlobalBounds().width > bounds.width) {
    newText.erase(newText.size() - 3, 1);
    text.setString(newText);
  }

  int newWidth = text.getGlobalBounds().width;
  text.setPosition(text.getPosition().x + oldWidth / 2 - newWidth / 2,
                   text.getPosition().y);
}

void drawText(RenderWindow &window, Text text) { window.draw(text); }
