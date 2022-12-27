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
            moveCursor(activeInput, -1);
          }
          // move the cursor of the input to the right
          else if (event.key.code == Keyboard::Right) {
            moveCursor(activeInput, 1);
          }
        }
        break;
      case Event::TextEntered:
          if (activeInput) {
          char enteredChar = event.text.unicode;

          // backspace
          if (enteredChar == 8) {
            eraseChar(activeInput);
                  }
          // enter
          else if (enteredChar == 13) {
            cout << activeInput->value << '\n';
            }
          // normal characters
          else if (31 < enteredChar && enteredChar < 128) {
            insertChar(activeInput, enteredChar);
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
