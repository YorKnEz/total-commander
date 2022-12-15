#include "element.h"

// checks if a certain box (button, text, file, etc.) is hoeverd by the mouse
bool isHovered(FloatRect box, int mouseX, int mouseY) {
  if ((box.left <= mouseX && mouseX <= box.left + box.width) &&
      (box.top <= mouseY && mouseY <= box.top + box.height)) {
    return true;
  }

  return false;
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
  textbox.background.setSize(Vector2f(width, height));
  textbox.background.setFillColor(bg);
  textbox.background.setOutlineColor(border);
  textbox.background.setOutlineThickness(borderThickness);
  textbox.background.setPosition(x, y);

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
  button.background.setSize(Vector2f(width, height));
  button.background.setFillColor(stateColors[B_INACTIVE].background);
  button.background.setOutlineColor(stateColors[B_INACTIVE].border);
  button.background.setOutlineThickness(borderThickness);
  button.background.setPosition(x, y);

  // initialize the text of the button
  button.text = createText(text, font, charSize, x, y,
                           button.background.getGlobalBounds().width -
                               2 * (borderThickness + 1),
                           stateColors[B_INACTIVE].text);

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
  button.text.setFillColor(button.stateColors[button.state].text);
  button.background.setFillColor(button.stateColors[button.state].background);
  button.background.setOutlineColor(button.stateColors[button.state].border);

  // draw the button on the window passed by reference
  window.draw(button.background);
  window.draw(button.text);
}

File createFile(Filedata data, Font &font, int charSize, int x, int y,
                int width, int height,
                FileStateColors stateColors[B_MAX_STATES],
                int borderThickness) {
  File file;
  file.state = B_INACTIVE; // set the state of the button
  // copy the state colors
  for (int i = 0; i < B_MAX_STATES; i++) {
    file.stateColors[i] = stateColors[i];
  }

  // initialize background
  file.background.setSize(Vector2f(width, height));
  file.background.setFillColor(file.stateColors[file.state].background);
  file.background.setOutlineColor(file.stateColors[file.state].border);
  file.background.setOutlineThickness(borderThickness);
  file.background.setPosition(x, y);

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
                     File *&activeFile) {
  FloatRect fileBounds =
      file.background.getGlobalBounds(); // get bounds of button

  cout << &file << " " << activeFile << '\n';

  switch (type) {
  // case RELEASE:
  //   if (file.state == B_CLICKED || file.state == B_DCLICKED) {
  //     file.state =
  //         isHovered(fileBounds, event.mouseButton.x, event.mouseButton.y)
  //             ? B_HOVERED
  //             : B_INACTIVE;
  //   }
  //   break;
  case DCLICK:
    // if a double click happens, then we execute the if, else we jump to the
    // simple click case, which is guarenteed to handle the event
    if (isHovered(fileBounds, event.mouseButton.x, event.mouseButton.y) &&
        isHovered(activeFile->background.getGlobalBounds(), event.mouseButton.x,
                  event.mouseButton.y)) {
      activeFile->state = B_DCLICKED;
      break;
    }
  case CLICK:
    if (isHovered(fileBounds, event.mouseButton.x, event.mouseButton.y)) {
      if (activeFile) {
        activeFile->state = B_INACTIVE;
      }

      activeFile = &file;
      activeFile->state = B_CLICKED;
    }
    break;
    // case MOVE:
    //   if (file.state != B_CLICKED && file.state != B_DCLICKED) {
    //     file.state = isHovered(fileBounds, event.mouseMove.x,
    //     event.mouseMove.y)
    //                      ? B_HOVERED
    //                      : B_INACTIVE;
    //   }
    //   break;
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
  input.background.setSize(Vector2f(width, height));
  input.background.setFillColor(stateColors[I_INACTIVE].background);
  input.background.setOutlineColor(stateColors[I_INACTIVE].border);
  input.background.setOutlineThickness(borderThickness);
  input.background.setPosition(x, y);

  // set the text to display on the screen
  string displayText = value == "" ? placeholder : value;
  input.displayText = createText(displayText, font, charSize, x + 10, y,
                                 input.background.getGlobalBounds().width -
                                     2 * (borderThickness + 1) - 10,
                                 stateColors[I_INACTIVE].text);

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

// expand the text to avoid the text being too small for the input
void expandInput(Input &input) {
  if (input.displayLength == input.value.size()) {
    return;
  }

  while (input.displayText.getGlobalBounds().width <
         input.background.getGlobalBounds().width - 20) {
    // if there is text to the right of the displayed text, add it
    if (input.startPosition + input.displayLength < input.value.size()) {
      input.displayLength++;
    }
    // if there is no text to the right
    else if (input.startPosition + input.displayLength == input.value.size()) {
      // expand from the left if possible
      if (input.startPosition > 0) {
        input.cursorLocation++;
        input.displayLength++;
        input.startPosition--;
      }
    }

    input.displayText.setString(
        input.value.substr(input.startPosition,
                           input.displayLength)); // update string
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

  // int radius = 10;
  //
  // CircleShape c(radius);
  // c.setFillColor(input.inputStateColors[input.state].background);
  // c.setOutlineThickness(input.background.getOutlineThickness());
  // c.setOutlineColor(input.inputStateColors[input.state].primary);
  //
  // RectangleShape sq(Vector2f(2 * radius, 2 * radius));
  // sq.setFillColor(Color(0x242424FF));
  // sq.setOutlineThickness(input.background.getOutlineThickness());
  // sq.setOutlineColor(Color(0x242424FF));
  //
  // FloatRect bgBounds = input.background.getGlobalBounds();
  //
  // // clear the corners
  // sq.setPosition(bgBounds.left, bgBounds.top);
  // window.draw(sq);
  // sq.setPosition(bgBounds.left + bgBounds.width - 2 * radius, bgBounds.top);
  // window.draw(sq);
  // sq.setPosition(bgBounds.left, bgBounds.top + bgBounds.height - 2 * radius);
  // window.draw(sq);
  // sq.setPosition(bgBounds.left + bgBounds.width - 2 * radius,
  //                bgBounds.top + bgBounds.height - 2 * radius);
  // window.draw(sq);
  //
  // // round the edges
  // c.setPosition(bgBounds.left, bgBounds.top);
  // window.draw(c);
  // c.setPosition(bgBounds.left + bgBounds.width - 2 * radius, bgBounds.top);
  // window.draw(c);
  // c.setPosition(bgBounds.left, bgBounds.top + bgBounds.height - 2 * radius);
  // window.draw(c);
  // c.setPosition(bgBounds.left + bgBounds.width - 2 * radius,
  //               bgBounds.top + bgBounds.height - 2 * radius);
  // window.draw(c);
  //
  // // fill the input
  // sq.setFillColor(input.inputStateColors[input.state].background);
  // sq.setOutlineThickness(input.background.getOutlineThickness());
  // sq.setOutlineColor(input.inputStateColors[input.state].background);
  //
  // sq.setSize(Vector2f(bgBounds.width, bgBounds.height - 2 * radius));
  // sq.setPosition(Vector2f(bgBounds.left, bgBounds.top + radius));
  //
  // window.draw(sq);
  //
  // sq.setSize(Vector2f(bgBounds.width - 2 * radius, bgBounds.height));
  // sq.setPosition(Vector2f(bgBounds.left + radius, bgBounds.top));
  //
  // window.draw(sq);
  //
  // // draw the borders
  // sq.setFillColor(input.inputStateColors[input.state].primary);
  // sq.setOutlineThickness(0);
  //
  // sq.setSize(Vector2f(input.background.getOutlineThickness(),
  //                     bgBounds.height - 2 * radius + 2));
  // sq.setPosition(Vector2f(bgBounds.left - 1, bgBounds.top + radius - 1));
  // window.draw(sq);
  //
  // sq.setPosition(
  //     Vector2f(bgBounds.left + bgBounds.width, bgBounds.top + radius - 1));
  // window.draw(sq);
  //
  // sq.setSize(Vector2f(bgBounds.width - 2 * radius + 2,
  //                     input.background.getOutlineThickness()));
  // sq.setPosition(Vector2f(bgBounds.left + radius - 1, bgBounds.top - 1));
  // window.draw(sq);
  //
  // sq.setPosition(
  //     Vector2f(bgBounds.left + radius - 1, bgBounds.top + bgBounds.height));
  // window.draw(sq);

  // initialize the text of the input
  if (input.value == "") {
    input.displayText.setString(input.placeholder);
  } else {
    input.displayText.setString(
        input.value.substr(input.startPosition, input.displayLength));
  }

  // expandInput(input);
  // shrinkInput(input);

  window.draw(input.displayText);

  // temp
  if (input.state == I_ACTIVE) {
    int charSize = input.displayText.getCharacterSize();
    Vector2f textPos = input.displayText.findCharacterPos(input.cursorLocation);
    FloatRect inputBounds = input.background.getGlobalBounds();

    RectangleShape r(Vector2f(1, 9 * charSize / 10));
    r.setFillColor(input.stateColors[input.state].text);
    r.setPosition(Vector2f(textPos.x, inputBounds.top + inputBounds.height / 2 -
                                          r.getGlobalBounds().height / 2));

    window.draw(r);
  }
}
