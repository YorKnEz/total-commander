#include "textbox.h"

TextBox createTextBox(string textString, Font &font, int charSize, int x, int y,
                      int width, int height, Color text, Color bg, Color border,
                      int borderThickness) {
  TextBox textbox;
  textbox.fullText =
      textString; // save the string that is supposed to be shown on the screen

  // initialize the background of the textbox
  textbox.background =
      createRect(x, y, width, height, bg, border, borderThickness);

  // initialize the text of the button
  textbox.text = createText(textString, font, charSize, x, y,
                            textbox.background.getGlobalBounds().width -
                                2 * (borderThickness + 1),
                            text);

  // set the offset to the text relative to the button
  int offsetX = textbox.background.getGlobalBounds().left +
                textbox.background.getGlobalBounds().width / 2 -
                textbox.text.getGlobalBounds().left -
                textbox.text.getGlobalBounds().width / 2;

  int offsetY = textbox.background.getGlobalBounds().top +
                textbox.background.getGlobalBounds().height / 2 -
                textbox.text.getGlobalBounds().top -
                textbox.text.getGlobalBounds().height / 2;

  textbox.text.setPosition(x + offsetX, y + offsetY);

  return textbox;
}

void drawTextBox(RenderWindow &window, TextBox textbox) {
  window.draw(textbox.background);
  window.draw(textbox.text);
}
