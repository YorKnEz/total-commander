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

void drawText(RenderWindow &window, Text text) { window.draw(text); }

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

void drawTextBox(RenderWindow &window, TextBox textbox) {
  window.draw(textbox.background);
  window.draw(textbox.text);
}

Button createButton(string text, Font &font, int charSize, int x, int y,
                    int width, int height,
                    StateColors stateColors[B_MAX_STATES],
                    unsigned int borderThickness) {
  Button button;
  button.state = B_INACTIVE; // set the state of the button
  button.fullText =
      text; // save the string that is supposed to be shown on the screen

  // copy the state colors
  for (int i = 0; i < B_MAX_STATES; i++) {
    button.stateColors[i] = stateColors[i];
  }

  // initialize the background of the button
  button.background =
      createRect(x, y, width, height, stateColors[button.state].background,
                 stateColors[button.state].border, borderThickness);

  // initialize the text of the button
  button.text = createText(text, font, charSize, x, y,
                           button.background.getGlobalBounds().width -
                               2 * (borderThickness + 1),
                           stateColors[button.state].text);

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
                       Vector2i &oldClick) {
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
        isHovered(buttonBounds, oldClick.x, oldClick.y)) {
      button.state = B_DCLICKED;
      oldClick = Vector2i(event.mouseButton.x, event.mouseButton.y);
      break;
    }
  case CLICK:
    button.state =
        isHovered(buttonBounds, event.mouseButton.x, event.mouseButton.y)
            ? B_CLICKED
            : B_INACTIVE;

    if (button.state == B_CLICKED) {
      oldClick = Vector2i(event.mouseButton.x, event.mouseButton.y);
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
  button.text.setFillColor(button.stateColors[button.state].text);
  button.background.setFillColor(button.stateColors[button.state].background);
  button.background.setOutlineColor(button.stateColors[button.state].border);

  // draw the button on the window passed by reference
  window.draw(button.background);
  window.draw(button.text);
}

File createFile(Filedata data, Font &font, int charSize, int x, int y,
                int width, int height,
                FileStateColors stateColors[F_MAX_STATES],
                int borderThickness) {
  File file;
  file.state = F_INACTIVE; // set the state of the button
  // copy the state colors
  for (int i = 0; i < F_MAX_STATES; i++) {
    file.stateColors[i] = stateColors[i];
  }

  // initialize background
  file.background =
      createRect(x, y, width, height, file.stateColors[file.state].background,
                 file.stateColors[file.state].border, borderThickness);

  // initialize data
  file.data = data;

  // initialize column sizes
  Text date("dd/mm/yyyy hh:mm xx", font, charSize);

  file.dateColumn = date.getGlobalBounds().width + 10;
  file.extColumn = (width - file.dateColumn) / 4;
  file.filenameColumn = (width - file.dateColumn) / 2;
  file.sizeColumn = (width - file.dateColumn) / 4;

  // initialize text fields
  int nameX = x + 10, extX = nameX + file.filenameColumn,
      sizeX = extX + file.extColumn, dateX = sizeX + file.sizeColumn;

  file.filename = createText(data.filename, font, charSize, nameX, y,
                             file.filenameColumn - 10,
                             file.stateColors[file.state].textHighContrast);
  file.ext = createText(data.ext, font, charSize, extX, y, file.extColumn - 10,
                        file.stateColors[file.state].textLowContrast);
  file.size =
      createText(data.size, font, charSize, sizeX, y, file.sizeColumn - 10,
                 file.stateColors[file.state].textLowContrast);
  file.date =
      createText(data.date, font, charSize, dateX, y, file.dateColumn - 10,
                 file.stateColors[file.state].textLowContrast);

  return file;
}

void updateFileState(File &file, Event event, MouseEventType type,
                     File *activeFile[2]) {
  FloatRect fileBounds =
      file.background.getGlobalBounds(); // get bounds of button
  Keyboard kbd;                          // used for checking combos

  switch (type) {
  case DCLICK:
    // if a dclick happens, let the explorer handle it
    if (activeFile[0] == &file) {
      break;
    }
  case CLICK:
    // if there is a click on a file
    if (isHovered(fileBounds, event.mouseButton.x, event.mouseButton.y)) {
      // if no file has been selected, set first file as crt file
      if (!activeFile[0]) {
        activeFile[0] = &file;
        activeFile[0]->state = F_SELECTED;
      }
      // if the first file has been selected, select the second
      else if (!activeFile[1]) {
        // select the second if shift key is also pressed
        if (kbd.isKeyPressed(Keyboard::LShift) && activeFile[0] != &file) {
          activeFile[1] = &file;
          activeFile[1]->state = F_SELECTED;
        }
        // select the first if the user just clicked
        else {
          activeFile[0]->state = F_INACTIVE;
          activeFile[0] = &file;
          activeFile[0]->state = F_SELECTED;
        }
      }
      // if both have been selected, reset them
      else {
        // select the second if shift key is pressed
        if (kbd.isKeyPressed(Keyboard::LShift) && activeFile[0] != &file) {
          activeFile[1]->state = F_INACTIVE;
          activeFile[1] = &file;
          activeFile[1]->state = F_SELECTED;
        }
        // reset both if not
        else {
          activeFile[0]->state = F_INACTIVE;
          activeFile[0] = &file;
          activeFile[0]->state = F_SELECTED;

          activeFile[1]->state = F_INACTIVE;
          activeFile[1] = nullptr;
        }
      }
    }
    break;
  }
}

void drawFile(RenderWindow &window, File file) {
  // update the color of the file depending on it's state
  file.filename.setFillColor(file.stateColors[file.state].textHighContrast);
  file.ext.setFillColor(file.stateColors[file.state].textLowContrast);
  file.size.setFillColor(file.stateColors[file.state].textLowContrast);
  file.date.setFillColor(file.stateColors[file.state].textLowContrast);

  file.background.setFillColor(file.stateColors[file.state].background);
  file.background.setOutlineColor(file.stateColors[file.state].border);

  window.draw(file.background);
  drawText(window, file.filename);
  drawText(window, file.ext);
  drawText(window, file.size);
  drawText(window, file.date);
}

Input createInput(string placeholder, string value, Font &font, int charSize,
                  int x, int y, int width, int height,
                  StateColors stateColors[I_MAX_STATES],
                  unsigned int borderThickness) {
  Input input;
  input.state = I_INACTIVE;        // set the state of the input
  input.placeholder = placeholder; // text to show if value is empty
  input.value = value;             // text that the user types
  input.cursorLocation = 0;
  // copy the state colors
  for (int i = 0; i < I_MAX_STATES; i++) {
    input.stateColors[i] = stateColors[i];
  }

  // initialize the background of the input
  input.background =
      createRect(x, y, width, height, stateColors[input.state].background,
                 stateColors[input.state].border, borderThickness);

  // set the text to display on the screen
  string displayText = value == "" ? placeholder : value;
  input.displayText = createText(displayText, font, charSize, x + 10, y,
                                 input.background.getGlobalBounds().width -
                                     2 * (borderThickness + 1) - 10,
                                 stateColors[input.state].text);

  // display only a segment of the text so it fits in the text box
  input.startPosition = 0;
  input.displayLength = input.value.size();

  input.displayText.setString(displayText);

  while (input.displayText.getGlobalBounds().width >
         input.background.getGlobalBounds().width - 10) {
    input.startPosition++;
    input.displayLength--;
    input.displayText.setString(
        displayText.substr(input.startPosition, input.displayLength));
  }

  int offsetY = input.background.getGlobalBounds().top +
                input.background.getGlobalBounds().height / 2 -
                input.displayText.getGlobalBounds().top -
                input.displayText.getGlobalBounds().height / 2;

  input.displayText.setPosition(x + 10, y + offsetY);

  return input;
}

void updateInputState(Input &input, Event event, MouseEventType type,
                      Input *&activeInput) {
  FloatRect inputBounds = input.background.getGlobalBounds();

  switch (type) {
  case CLICK:
    // update the currently active input
    if (isHovered(inputBounds, event.mouseButton.x, event.mouseButton.y)) {
      activeInput = &input;
      activeInput->state = I_ACTIVE;
    }

    break;
  case MOVE:
    if (input.state != I_ACTIVE) {
      input.state = isHovered(inputBounds, event.mouseMove.x, event.mouseMove.y)
                        ? I_HOVERED
                        : I_INACTIVE;
    }
    break;
  }
}

// shrink the text to avoid overflow
void shrinkInput(Input &input) {
  while (input.displayText.getGlobalBounds().width >
         input.background.getGlobalBounds().width - 20) {
    // if the cursor is at the end of the input
    if (input.cursorLocation == input.displayLength) {
      input.cursorLocation--; // move cursor back
      input.displayLength--;  // shrink display length
      input.startPosition++;  // advance start position
    }
    // if the cursor is anywhere before the end
    else {
      input.displayLength--; // shrink display length
    }

    input.displayText.setString(
        input.value.substr(input.startPosition,
                           input.displayLength)); // update string
  }
}

void drawInput(RenderWindow &window, Input &input) {
  // update the color of the input depending on it's state
  input.displayText.setFillColor(input.stateColors[input.state].text);
  input.background.setFillColor(input.stateColors[input.state].background);
  input.background.setOutlineColor(input.stateColors[input.state].border);

  // draw the input on the window
  window.draw(input.background);

  // initialize the text of the input
  if (input.value == "") {
    input.displayText.setString(input.placeholder);
  } else {
    input.displayText.setString(
        input.value.substr(input.startPosition, input.displayLength));
  }

  // shrinkInput(input);

  window.draw(input.displayText);
}

void drawCursor(RenderWindow &window, Input *activeInput) {
  if (activeInput && activeInput->state == I_ACTIVE) {
    int charSize = activeInput->displayText.getCharacterSize();
    Vector2f textPos =
        activeInput->displayText.findCharacterPos(activeInput->cursorLocation);
    FloatRect inputBounds = activeInput->background.getGlobalBounds();

    RectangleShape r(Vector2f(1, 9 * charSize / 10));
    r.setFillColor(activeInput->stateColors[activeInput->state].text);
    r.setPosition(Vector2f(textPos.x, inputBounds.top + inputBounds.height / 2 -
                                          r.getGlobalBounds().height / 2));

    window.draw(r);
  }
}

Scrollbar createScrollbar(Font &font, int charSize, int x, int y, int width,
                          int height, int btnHeight, int scrollableHeight,
                          StateColors stateColors[B_MAX_STATES],
                          unsigned int borderThickness) {
  Scrollbar scrollbar;

  scrollbar.state = B_INACTIVE;
  scrollbar.scrollableHeight = scrollableHeight;
  scrollbar.scrollOffset = 0;

  scrollbar.up = createButton("/\\", font, charSize, x, y, width, btnHeight,
                              stateColors, borderThickness);
  scrollbar.down = createButton("\\/", font, charSize, x,
                                y + height - btnHeight + 2 * borderThickness,
                                width, btnHeight, stateColors, borderThickness);

  int trackHeight = height - 2 * btnHeight;

  scrollbar.track =
      createRect(x, y + btnHeight + 2 * borderThickness, width,
                 trackHeight - 2 * borderThickness,
                 stateColors[scrollbar.state].background,
                 stateColors[scrollbar.state].border, borderThickness);

  int displayHeight = trackHeight + btnHeight + borderThickness;
  int thumbHeight =
      (float(displayHeight) / float(scrollableHeight)) * float(trackHeight);

  if (thumbHeight > trackHeight) {
    thumbHeight = trackHeight;
  }

  scrollbar.thumb = createRect(x, y + btnHeight + 2 * borderThickness, width,
                               thumbHeight, stateColors[scrollbar.state].text,
                               stateColors[scrollbar.state].border, 0);

  return scrollbar;
}

void updateScrollableHeight(Scrollbar &scrollbar, int scrollableHeight) {
  FloatRect bounds = scrollbar.thumb.getGlobalBounds();

  int trackHeight = scrollbar.track.getGlobalBounds().height;
  int displayHeight =
      trackHeight + scrollbar.down.background.getGlobalBounds().height;
  int thumbHeight =
      (float(displayHeight) / float(scrollableHeight)) * float(trackHeight);

  if (thumbHeight > trackHeight) {
    thumbHeight = trackHeight;
  }

  scrollbar.thumb.setSize(Vector2f(bounds.width, thumbHeight));

  scrollbar.scrollableHeight = scrollableHeight;
}

void updateScrollbar(Scrollbar &scrollbar, int scrollOffset) {
  float ratio =
      scrollbar.track.getGlobalBounds().height / scrollbar.scrollableHeight;

  Vector2f position = scrollbar.track.getPosition();

  scrollbar.scrollOffset = -ratio * float(scrollOffset);
  scrollbar.thumb.setPosition(
      Vector2f(position.x, position.y + scrollbar.scrollOffset));
}

void updateScrollbarOnDrag(Scrollbar &scrollbar, int &scrollOffset) {
  float ratio =
      scrollbar.track.getGlobalBounds().height / scrollbar.scrollableHeight;

  Vector2f position = scrollbar.track.getPosition();

  scrollbar.scrollOffset += scrollOffset;
  scrollbar.thumb.setPosition(
      Vector2f(position.x, position.y + scrollbar.scrollOffset));

  // update the scrollOffset to the proper ratio
  scrollOffset = float(scrollOffset) / (-ratio);
}

void drawScrollbar(RenderWindow &window, Scrollbar scrollbar) {
  drawButton(window, scrollbar.up);
  drawButton(window, scrollbar.down);

  window.draw(scrollbar.track);
  window.draw(scrollbar.thumb);
}
