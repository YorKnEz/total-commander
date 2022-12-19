#include <menu.h>

string createPopUp(int width, int height, PopUpType type, string windowTitle,
                   string textBoxString, string buttonString,
                   string inputPlaceholder, string inputValue, Font &font,
                   int charSize, ColorTheme theme) {
  RenderWindow window(VideoMode(width, height), windowTitle);
  // center window on the screen
  window.setPosition(
      Vector2i(VideoMode::getDesktopMode().width / 2 - width / 2,
               VideoMode::getDesktopMode().height / 2 - height / 2));
  Vector2i oldClick; // the bounds of the last click
  RectangleShape cursor; // cursor to display on inputs
  Event event;
  string errorText;
  Input *activeInput = nullptr;
  TextBox textbox =
      createTextBox(textBoxString, font, charSize, 0, 0, width, height / 4,
                    theme.textMediumContrast, theme.bgBody, theme.border, 0);
  textbox.text.setPosition(20, textbox.text.getPosition().y + 20);
  Button button =
      createButton(buttonString, font, charSize, width / 2 - width / 8,
                   height - 50, width / 4, 30, theme.buttonStateColors, 1);
  Input input = createInput(inputPlaceholder, inputValue, font, charSize, 20,
                            height / 2 - height / 8, width - 40, height / 4,
                            theme.inputStateColors, 1);
  while (window.isOpen()) {
    while (window.pollEvent(event)) {
      switch (event.type) {
      case Event::Closed:
        window.close();
        return "";
      case Event::KeyPressed:
        // input related keys
        if (activeInput) {
          // move the cursor of the input to the left
          if (event.key.code == Keyboard::Left) {
            // try moving the cursor first
            if (activeInput->cursorLocation > 0) {
              activeInput->cursorLocation--;
            }
            // if the cursor is at the start of the input, decrease start pos
            else if (activeInput->startPosition > 0) {
              activeInput->startPosition--;
            }
          }
          // move the cursor of the input to the right
          else if (event.key.code == Keyboard::Right) {
            // try moving the cursor first
            if (activeInput->cursorLocation < activeInput->displayLength) {
              activeInput->cursorLocation++;
            }
            // if the cursor is at the end of the input, increase start pos
            else if (activeInput->startPosition + activeInput->displayLength <
                     activeInput->value.size()) {
              activeInput->startPosition++;
            }
          }
        }
        break;
      case Event::TextEntered:
        if (event.text.unicode < 128) {
          if (activeInput) {
            // check if key pressed is backspace
            if (event.text.unicode == 8) {
              // delete the character before the cursor position
              if (activeInput->startPosition + activeInput->cursorLocation >
                  0) {
                activeInput->value.erase(activeInput->startPosition +
                                             activeInput->cursorLocation - 1,
                                         1);

                // first case: text is smaller than the input or equal
                if (activeInput->displayLength ==
                    activeInput->value.size() + 1) {
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
              // else add the character entered at the cursor position
            } else {
              activeInput->value.insert(activeInput->startPosition +
                                            activeInput->cursorLocation,
                                        1, char(event.text.unicode));

              // try to expand the displayed text if there is space in the
              // input
              if (activeInput->displayText.getGlobalBounds().width <
                  activeInput->background.getGlobalBounds().width - charSize) {
                activeInput->displayLength++;  // increse number of chars shown
                activeInput->cursorLocation++; // advance cursor

                activeInput->displayText.setString(activeInput->value.substr(
                    activeInput->startPosition,
                    activeInput->displayLength)); // update string
              }
              // if we can't expand the text, then we have to move it
              else {
                // if the cursor is at the end we need to advance the start
                // position
                if (activeInput->cursorLocation == activeInput->displayLength) {
                  activeInput->startPosition++;
                }
                // else advance only the cursor
                else {
                  activeInput->cursorLocation++;
                }
              }

              shrinkInput(*activeInput);
            }
          }
        }
        break;
      case Event::MouseButtonReleased:
        updateInputState(input, event, RELEASE, activeInput);
        updateButtonState(button, event, RELEASE, oldClick);

        break;
      case Event::MouseButtonPressed:
        // disable the last active input if user clicks outside it's box,
        // regardless of type of click
        if (activeInput &&
            !isHovered(activeInput->background.getGlobalBounds(),
                       event.mouseButton.x, event.mouseButton.y)) {
          activeInput->state = I_INACTIVE;
          activeInput = nullptr;
        }
        // simple click
        updateInputState(input, event, CLICK, activeInput);
        updateButtonState(button, event, CLICK, oldClick);

        if (button.state == B_CLICKED) {
          if (input.value == "") {
            createErrorPopUp(400, 150, "Error", "The input cannot be empty.",
                             font, charSize, theme);
          } else if (type == GET_FILENAME) {
            if (!isValidFilename(input.value) or input.value.size() > 100) {
              createErrorPopUp(400, 150, "Error", "The input is invalid.", font,
                               charSize, theme);
            } else {
              createErrorPopUp(400, 150, "Total Commander",
                               "The operation was successful.", font, charSize,
                               theme);
              window.close();
              return input.value;
            }
          } else {
            if (evalPath(input.value) == "invalid") {
              createErrorPopUp(400, 150, "Error", "The input is invalid.", font,
                               charSize, theme);
            } else {
              if (!isValidPath(input.value)) {
                createErrorPopUp(400, 150, "Error", "The input is invalid.",
                                 font, charSize, theme);
              } else {
                createErrorPopUp(400, 150, "Total Commander",
                                 "The operation was successful.", font,
                                 charSize, theme);
                window.close();
                return evalPath(input.value);
              }
            }
          }
        }

        break;

      case Event::MouseMoved:
        updateInputState(input, event, RELEASE, activeInput);
        updateButtonState(button, event, MOVE, oldClick);
        break;
      }
    }
    window.clear(theme.bgBody);
    drawTextBox(window, textbox);
    drawInput(window, input);
    drawButton(window, button);
    drawCursor(window, activeInput);
    window.display();
  }
  return "";
}

void createErrorPopUp(int width, int height, string windowTitle,
                      string errorText, Font &font, int charSize,
                      ColorTheme theme) {
  RenderWindow window(VideoMode(width, height), windowTitle);
  // center window on the screen
  window.setPosition(
      Vector2i(VideoMode::getDesktopMode().width / 2 - width / 2,
               VideoMode::getDesktopMode().height / 2 - height / 2));
  FloatRect oldClick; // the bounds of the last click
  RectangleShape cursor; // cursor to display on inputs
  Event event;
  Input *activeInput = nullptr;
  TextBox textbox = createTextBox(errorText, font, charSize, 0, 0, width,
                                  height, theme.textMediumContrast,
                                  theme.bgLowContrast, theme.border, 1);
  while (window.isOpen()) {
    while (window.pollEvent(event)) {
      switch (event.type) {
      case Event::Closed:
        window.close();
        break;
      }
      window.clear(theme.bgBody);
      drawTextBox(window, textbox);
      window.display();
    }
  }
}
