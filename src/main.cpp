#include "dllist.h"
#include "element.h"
#include "filetree.h"
#include "theme.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

#define TITLE "Total Commander"
#define WINDOW_W 1280
#define WINDOW_H 720

#define DCLICK_MAX_DELAY 300

using namespace sf;
using namespace std;

int main() {
  RenderWindow window(VideoMode(WINDOW_W, WINDOW_H), TITLE);

  ColorTheme theme = dark;

  Font font;
  font.loadFromFile("assets/calibri.ttf");

  int btnw = 100;
  int charSize = 20;

  Button buttons[11];

  for (int i = 1; i <= 10; i++) {
    buttons[i] =
        createButton("test12312", font, charSize, WINDOW_W - i * (btnw + 20),
                     WINDOW_H - 60, btnw, 40, theme.buttonStateColors, 1);
  }

  Input input = createInput("Enter text here", "", font, charSize, 20, 20, 400,
                            40, theme.inputStateColors, 1);
  Input input2 = createInput("Enter another text here", "", font, charSize, 20,
                             80, 400, 40, theme.inputStateColors, 1);

  // useful for determining double clicks
  Clock clock;           // a timer that is set between two clicks
  bool click = false;    // a flag which checks if we have a double click
  FloatRect clickBounds; // the bounds of the last click
  Input *activeInput = nullptr;
  RectangleShape cursor; // cursor to display on inputs

  list l;
  init(l);
#ifdef _WIN32
  string path = "x:\\";
  for (int i = 0; i < 26; i++) {
    path[0] = char(i + 'A');
    if (fs::exists(path))
      break;
  }
  path += "Proiect";
#endif
#ifdef linux
  string path = "home/Proiect/";
#endif

  getFilesFromPath(l, path);

  printList(l);

  node *p = l.head;
  File file;
  int fileY = 0;

  if (l.length >= 2)
    sortFiletree(l, FILE_DATE, ASC);

  while (window.isOpen()) {
    Event event;
    while (window.pollEvent(event)) {
      switch (event.type) {
      case Event::Closed:
        window.close();
        break;
      case Event::KeyPressed:
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
            else if (activeInput->startPosition > 0 &&
                     activeInput->startPosition + activeInput->displayLength -
                             1 <
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
                // activeInput->value.erase(activeInput->startPosition +
                //                              activeInput->cursorLocation - 1,
                //                          1);
              }

            }
            // else add the character entered at the cursor position
            else {
              activeInput->value.insert(activeInput->startPosition +
                                            activeInput->cursorLocation,
                                        1, char(event.text.unicode));

              cout << "Cursor: " << input.cursorLocation << " | ";
              cout << "Size: " << input.value.size() << " | ";
              cout << "Start pos: " << input.startPosition << " | ";
              cout << "Display len: " << input.displayLength << "\n\n";
            }
          }
        }
        break;
      case Event::MouseButtonReleased:
        for (int i = 1; i <= 10; i++) {
          updateButtonState(buttons[i], event, RELEASE, clickBounds);
        }
        updateInputState(input, event, RELEASE, activeInput);
        updateInputState(input2, event, RELEASE, activeInput);
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

        updateInputState(input, event, CLICK, activeInput);
        updateInputState(input2, event, CLICK, activeInput);

        // double click
        if (click &&
            clock.getElapsedTime().asMilliseconds() <= DCLICK_MAX_DELAY) {
          for (int i = 1; i <= 10; i++) {
            updateButtonState(buttons[i], event, DCLICK, clickBounds);

            // we reset the click flag only if a double click happened
            if (buttons[i].state == B_DCLICKED) {
          click = false;
            }
            // we reset the click timer only if a click happened
            else if (buttons[i].state == B_CLICKED) {
              clock.restart();
            }
          }
        }
        // simple click
        else {
          for (int i = 1; i <= 10; i++) {
            updateButtonState(buttons[i], event, CLICK, clickBounds);
          }

          // start the timer and set the click flag to true if a click happens
          click = true;
          clock.restart();
        }

        break;
      case Event::MouseMoved:
        for (int i = 1; i <= 10; i++) {
          updateButtonState(buttons[i], event, MOVE, clickBounds);
        }
        updateInputState(input, event, MOVE, activeInput);
        updateInputState(input2, event, MOVE, activeInput);
        break;
      }
    }

    window.clear(theme.background);
    for (int i = 1; i <= 10; i++) {
      drawButton(window, buttons[i]);
    }
    drawInput(window, input);
    drawInput(window, input2);

    p = l.head;
    fileY = 0;

    while (p) {
      file = createFile(p->data, font, 20, 20, fileY, WINDOW_W / 2, 40,
                        theme.text);

      drawFile(window, file);

      fileY += file.background.getGlobalBounds().height;
      p = p->next;
    }

    window.display();
  }

  return 0;
}
