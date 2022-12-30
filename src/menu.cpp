#include "menu.h"

string createPopUp(int width, int height, PopUpType type, string windowTitle,
                   string textBoxString, string buttonString,
                   string inputPlaceholder, string inputValue, Font &font,
                   int charSize, ColorTheme theme) {
  RenderWindow window(VideoMode(width, height), windowTitle);
  // center window on the screen
  window.setPosition(
      Vector2i(VideoMode::getDesktopMode().width / 2 - width / 2,
               VideoMode::getDesktopMode().height / 2 - height / 2));

  Vector2i oldClick;     // the bounds of the last click
  RectangleShape cursor; // cursor to display on inputs
  string errorText;
  Input *activeInput = nullptr;

  int compHeight = 30, btnWidth = 120, center = height / 2 - compHeight / 2;

  TextBox textbox = createTextBox(
      textBoxString, font, charSize, 20, center - 10 - compHeight, width - 40,
      compHeight, theme.textMediumContrast, theme.bgBody, theme.border, 0);

  Input input =
      createInput(inputPlaceholder, inputValue, font, charSize, 20, center,
                  width - 40 - 2, compHeight - 2, theme.inputStateColors, 1);

  Button button =
      createButton(buttonString, font, charSize, width / 2 - btnWidth / 2,
                   center + compHeight + 10, btnWidth, compHeight,
                   theme.buttonStateColors, 1);

  while (window.isOpen()) {
    Event event;
    while (window.pollEvent(event)) {
      switch (event.type) {
      case Event::Closed:
        window.close();
        return "";
      case Event::KeyPressed:
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
            createErrorPopUp(POP_UP_DEFAULT_W, POP_UP_DEFAULT_H, "Error",
                             "The input cannot be empty.", font, charSize,
                             theme);
          } else if (type == GET_FILENAME) {
            if (!isValidFilename(input.value) or input.value.size() > 100) {
              createErrorPopUp(POP_UP_DEFAULT_W, POP_UP_DEFAULT_H, "Error",
                               "The input is invalid.", font, charSize, theme);
            } else {
              createErrorPopUp(
                  POP_UP_DEFAULT_W, POP_UP_DEFAULT_H, "Total Commander",
                  "The operation was successful.", font, charSize, theme);
              window.close();
              return input.value;
            }
          } else {
            if (evalPath(input.value) == "invalid") {
              createErrorPopUp(POP_UP_DEFAULT_W, POP_UP_DEFAULT_H, "Error",
                               "The input is invalid.", font, charSize, theme);
            } else {
              if (!isValidPath(input.value)) {
                createErrorPopUp(POP_UP_DEFAULT_W, POP_UP_DEFAULT_H, "Error",
                                 "The input is invalid.", font, charSize,
                                 theme);
              } else {
                createErrorPopUp(
                    POP_UP_DEFAULT_W, POP_UP_DEFAULT_H, "Total Commander",
                    "The operation was successful.", font, charSize, theme);
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

  FloatRect oldClick;    // the bounds of the last click
  RectangleShape cursor; // cursor to display on inputs
  Input *activeInput = nullptr;

  TextBox textbox = createTextBox(errorText, font, charSize, 0, 0, width,
                                  height, theme.textMediumContrast,
                                  theme.bgLowContrast, theme.border, 1);

  while (window.isOpen()) {
    Event event;
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

void handleMenuButtons(Explorer *explorer, int explorers,
                       Explorer *activeExplorer, MenuButtons button,
                       string windowTitle, Font &font, int charSize,
                       ColorTheme theme) {
  string newPath = "";
  string newName, currentEntryName, currentEntryExt, currentEntry;

  switch (button) {
  case MKDIR:
    newName = createPopUp(POP_UP_DEFAULT_W, POP_UP_DEFAULT_H, GET_FILENAME,
                          windowTitle, "Folder name: ", "OK", "Folder name", "",
                          font, charSize, theme);

    if (!newName.empty()) {
      createFolder(activeExplorer->path, newName);
    }

    break;
  case COPY_ENTRY:
    if (activeExplorer->activeFile[0]) {
      newPath = createPopUp(POP_UP_DEFAULT_W, POP_UP_DEFAULT_H, GET_PATH,
                            windowTitle, "Copy to:", "OK", "Destination path",
                            activeExplorer->path, font, charSize, theme);

      if (!newPath.empty()) {
        node *p = activeExplorer->files.head;

        while (p) {
          if (p->data.state == F_SELECTED &&
              p->data.data.filename.compare("..")) {
            currentEntryName = p->data.data.filename;
            currentEntryExt = p->data.data.ext;
            currentEntry = currentEntryName;

            // add the extension to the crt entry if it exists
            if (!currentEntryExt.empty()) {
              currentEntry += "." + currentEntryExt;
            }

            copyEntry(activeExplorer->path + SEP + currentEntry, newPath);
          }

          p = p->next;
        }
      }
    }

    break;
  case DELETE_ENTRY:
    if (activeExplorer->activeFile[0]) {
      node *p = activeExplorer->files.head;

      while (p) {
        if (p->data.state == F_SELECTED &&
            p->data.data.filename.compare("..")) {
          currentEntryName = p->data.data.filename;
          currentEntryExt = p->data.data.ext;
          currentEntry = currentEntryName;

          // add the extension to the crt entry if it exists
          if (!currentEntryExt.empty()) {
            currentEntry += "." + currentEntryExt;
          }

          deleteEntry(activeExplorer->path + SEP + currentEntry);
        }

        p = p->next;
      }
    }

    break;
  case MOVE_ENTRY:
    if (activeExplorer->activeFile[0]) {
      newPath = createPopUp(POP_UP_DEFAULT_W, POP_UP_DEFAULT_H, GET_PATH,
                            windowTitle, "Move to:", "OK", "Destination path",
                            activeExplorer->path, font, charSize, theme);

      if (!newPath.empty()) {
        node *p = activeExplorer->files.head;

        while (p) {
          if (p->data.state == F_SELECTED &&
              p->data.data.filename.compare("..")) {
            currentEntryName = p->data.data.filename;
            currentEntryExt = p->data.data.ext;
            currentEntry = currentEntryName;

            // add the extension to the crt entry if it exists
            if (!currentEntryExt.empty()) {
              currentEntry += "." + currentEntryExt;
            }

            moveEntry(activeExplorer->path + SEP + currentEntry, newPath);
          }

          p = p->next;
        }
      }
    }

    break;
  case RENAME_ENTRY:
    if (activeExplorer->activeFile[0]) {

      newName = createPopUp(POP_UP_DEFAULT_W, POP_UP_DEFAULT_H, GET_FILENAME,
                            windowTitle, "Rename to: ", "OK", "New name", "",
                            font, charSize, theme);

      if (!newName.empty()) {
        node *p = activeExplorer->files.head;

        while (p) {
          if (p->data.state == F_SELECTED &&
              p->data.data.filename.compare("..")) {
            currentEntryName = p->data.data.filename;
            currentEntryExt = p->data.data.ext;
            currentEntry = currentEntryName;

            // add the extension to the crt entry if it exists
            if (!currentEntryExt.empty()) {
              currentEntry += "." + currentEntryExt;
            }

            editEntryName(activeExplorer->path + SEP + currentEntry, newName);
          }

          p = p->next;
        }
      }
    }

    break;
  }

  for (int i = 0; i < explorers; i++) {
    if (explorer[i].path == activeExplorer->path ||
        explorer[i].path == newPath) {
      refreshExplorer(explorer[i], activeExplorer, font, theme);
    }
  }
}
