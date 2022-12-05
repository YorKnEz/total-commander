#include "element.h"

// checks if a certain box (button, text, file, etc.) is hoeverd by the mouse
bool isHovered(FloatRect box, int mouseX, int mouseY) {
  if ((box.left <= mouseX && mouseX <= box.left + box.width) &&
      (box.top <= mouseY && mouseY <= box.top + box.height)) {
    return true;
  }

  return false;
}

Button createButton(string text, Font &font, int charSize, int x, int y,
                    int width, int height,
                    ButtonStateColors buttonStateColors[B_MAX_STATES],
                    unsigned int borderThickness) {
  Button button;
  button.state = B_INACTIVE; // set the state of the button
  button.fullText =
      text; // save the string that is supposed to be shown on the screen

  // copy the state colors
  for (int i = 0; i < B_MAX_STATES; i++) {
    button.buttonStateColors[i] = buttonStateColors[i];
  }

  // initialize the background of the button
  button.background.setSize(Vector2f(width, height));
  button.background.setFillColor(buttonStateColors[B_INACTIVE].background);
  button.background.setOutlineColor(buttonStateColors[B_INACTIVE].primary);
  button.background.setOutlineThickness(borderThickness);
  button.background.setPosition(x, y);

  // initialize the text of the button
  button.text = createText(text, font, charSize, x, y,
                           button.background.getGlobalBounds().width -
                               2 * (borderThickness + 1),
                           buttonStateColors[B_INACTIVE].primary);

  // set the offset to the text relative to the button
  int offsetX = button.background.getGlobalBounds().left +
                button.background.getGlobalBounds().width / 2 -
                button.text.getGlobalBounds().left -
                button.text.getGlobalBounds().width / 2;

  int offsetY = button.background.getGlobalBounds().top +
                button.background.getGlobalBounds().height / 2 -
                button.text.getGlobalBounds().top -
                button.text.getGlobalBounds().height / 2;

  button.text.setPosition(x + offsetX, y + offsetY);

  return button;
}

void updateButtonState(Button &button, Event event, MouseEventType type,
                       FloatRect &clickBounds) {
  button.oldState = button.state; // update old state
  FloatRect buttonBounds =
      button.background.getGlobalBounds(); // get bounds of button

  switch (type) {
  case RELEASE:
    if (button.state == B_CLICKED || button.state == B_DCLICKED) {
      button.state =
          isHovered(buttonBounds, event.mouseButton.x, event.mouseButton.y)
              ? B_HOVERED
              : B_INACTIVE;
    }
    break;
  case DCLICK:
    // if a double click happens, then we execute the if, else we jump to the
    // simple click case, which is guarenteed to handle the event
    if (isHovered(buttonBounds, event.mouseButton.x, event.mouseButton.y) &&
        isHovered(clickBounds, event.mouseButton.x, event.mouseButton.y)) {
      button.state = B_DCLICKED;
      clickBounds = buttonBounds;
    break;
    }
  case CLICK:
    button.state =
        isHovered(buttonBounds, event.mouseButton.x, event.mouseButton.y)
            ? B_CLICKED
            : B_INACTIVE;
    if (button.state == B_CLICKED) {
      clickBounds = buttonBounds;
    }
    break;
  case MOVE:
    if (button.state != B_CLICKED && button.state != B_DCLICKED) {
      button.state =
          isHovered(buttonBounds, event.mouseMove.x, event.mouseMove.y)
              ? B_HOVERED
              : B_INACTIVE;
    }
    break;
  }
}

void drawButton(RenderWindow &window, Button button) {
  // update the color of the button depending on it's state
  button.text.setFillColor(button.buttonStateColors[button.state].primary);
  button.background.setOutlineColor(
      button.buttonStateColors[button.state].primary);

  // draw the button on the window passed by reference
  window.draw(button.background);
  window.draw(button.text);
}

Text createText(string textString, Font &font, int charSize, int x, int y,
                int width, Color textColor) {
  Text text;
  text.setFont(font);
  text.setPosition(x, y);
  text.setCharacterSize(charSize);
  text.setStyle(Text::Regular);
  text.setString(textString);
  text.setFillColor(textColor);
  // shrink the text that is shown on the screen to avoid overflow
  if (text.getGlobalBounds().width > width) {
    textString.append("..");
    text.setString(textString);
  }

  while (text.getGlobalBounds().width > width) {
    textString.erase(textString.size() - 3, 1);
    text.setString(textString);
  }
  return text;
}

void drawText(RenderWindow &window, Text text) { window.draw(text); }

File createFile(Filedata data, Font &font, int charSize, int x, int y,
                int width, int height, Color textColor) {
  File file;
  file.background.setSize(Vector2f(width, height));
  file.background.setFillColor(Color(0x123456FF));
  file.background.setPosition(x, y);

  int dateColumn, extColumn, filenameColumn, sizeColumn;
  Text date("dd/mm/yyyy hh:mm xx", font, charSize);

  dateColumn = date.getGlobalBounds().width;
  extColumn = (width - dateColumn) / 4;
  filenameColumn = (width - dateColumn) / 2;
  sizeColumn = (width - dateColumn) / 4;

  file.filename = createText(data.filename, font, charSize, x, y,
                             filenameColumn, textColor);
  file.ext = createText(data.ext, font, charSize, x + filenameColumn, y,
                        extColumn, textColor);
  file.size =
      createText(data.size, font, charSize, x + filenameColumn + extColumn, y,
                 sizeColumn, textColor);
  file.date = createText(data.date, font, charSize,
                         x + filenameColumn + extColumn + sizeColumn, y,
                         dateColumn, textColor);

  return file;
}

void drawFile(RenderWindow &window, File file) {
  window.draw(file.background);
  drawText(window, file.filename);
  drawText(window, file.ext);
  drawText(window, file.size);
  drawText(window, file.date);
}

bool isHovered(File &file, int mouseX, int mouseY) {
  FloatRect fileBounds = file.background.getGlobalBounds();

  if ((fileBounds.left <= mouseX &&
       mouseX <= fileBounds.left + fileBounds.width) &&
      (fileBounds.top <= mouseY &&
       mouseY <= fileBounds.top + fileBounds.height)) {
    return true;
  }

  return false;
}