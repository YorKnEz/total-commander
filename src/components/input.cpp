#include "input.h"

Input createInput(string placeholder, string value, Font &font, int charSize,
                  int x, int y, int width, int height,
                  StateColors stateColors[I_MAX_STATES],
                  unsigned int borderThickness) {
  Input input;
  input.state = I_INACTIVE;        // set the state of the input
  input.placeholder = placeholder; // text to show if value is empty
  input.value = value;             // text that the user types
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
  input.cursorLocation = input.displayLength;

  input.displayText.setString(displayText);

  shrinkInput(input);

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

void updateInputText(Input &input, string newValue, string newPlaceholder) {
  // update the input of the explorer
  input.value = newValue;

  if (newPlaceholder.size() > 0) {
    input.placeholder = newPlaceholder;
  }

  input.displayText.setString(input.value == "" ? input.placeholder
                                                : input.value);
  input.displayLength = input.value.size();
  input.cursorLocation = input.value.size();
  input.startPosition = 0;

  shrinkInput(input);
}

void updateInputTheme(Input &input, StateColors stateColors[I_MAX_STATES]) {
  // copy the state colors
  for (int i = 0; i < I_MAX_STATES; i++) {
    input.stateColors[i] = stateColors[i];
  }

  input.background.setFillColor(stateColors[input.state].background);
  input.background.setOutlineColor(stateColors[input.state].border);

  input.displayText.setFillColor(stateColors[input.state].text);
}

void insertChar(Input *activeInput, char c) {
  activeInput->value.insert(
      activeInput->startPosition + activeInput->cursorLocation, 1, c);

  // expand the displayed text if there is space
  if (activeInput->displayText.getGlobalBounds().width <
      activeInput->background.getGlobalBounds().width -
          2 * activeInput->displayText.getCharacterSize()) {
    activeInput->displayLength++;  // increse number of chars shown
    activeInput->cursorLocation++; // advance cursor
  }
  // move the cursor after the added character
  else {
    moveCursor(activeInput, 1);
  }

  activeInput->displayText.setString(
      activeInput->value.substr(activeInput->startPosition,
                                activeInput->displayLength)); // update string
}

void eraseChar(Input *activeInput) {
  // delete the character before the cursor position
  if (activeInput->startPosition + activeInput->cursorLocation > 0) {
    activeInput->value.erase(
        activeInput->startPosition + activeInput->cursorLocation - 1, 1);

    // first case: text is smaller than the input or equal
    if (activeInput->displayLength == activeInput->value.size() + 1) {
      // a: the cursor is not at the beginning of the text
      if (activeInput->cursorLocation > 0) {
        activeInput->displayLength--; // shrink displayed text
        activeInput->cursorLocation--;
      }
    }
    // second case: text is larger than the input
    else {
      // a: there is no text to the right of the displayed text
      //    so try to expand the text from the left
      //
      //    being at this case implies that startPosition > 0
      if (activeInput->startPosition + activeInput->displayLength ==
          activeInput->value.size() + 1) {
        activeInput->startPosition--;
      }
      // b: there is text to the right of the displayed text
      //    so try to expand the input from the right
      else {
        // move cursor to the left if possible
        if (activeInput->cursorLocation > 0) {
          activeInput->cursorLocation--;
        }
        // move start position to the left if cursor can't be moved
        else {
          activeInput->startPosition--;
        }
      }
    }
  }

  string newValue = activeInput->value.substr(activeInput->startPosition,
                                              activeInput->displayLength);

  activeInput->displayText.setString(
      newValue == "" ? activeInput->placeholder : newValue); // update string
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
  window.draw(input.displayText);
}

void moveCursor(Input *activeInput, int direction) {
  // update the cursor's location only if it doesn't get out of the input's
  // bounds
  int newCursorLocation = activeInput->cursorLocation + direction;
  int newStartPosition = activeInput->startPosition + direction;

  if (0 <= newCursorLocation &&
      newCursorLocation <= activeInput->displayLength) {
    activeInput->cursorLocation = newCursorLocation;
  }
  // move the start position if cursor location can't be updated
  else if (0 <= newStartPosition &&
           newStartPosition + activeInput->displayLength <=
               activeInput->value.size()) {
    activeInput->startPosition = newStartPosition;
  }

  activeInput->displayText.setString(
      activeInput->value.substr(activeInput->startPosition,
                                activeInput->displayLength)); // update string
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
