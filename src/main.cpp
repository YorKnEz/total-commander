#include "explorer.h"
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

  ColorTheme dark = {Color(0xEEEEEEFF),
                     Color(0x242424FF),
                     Color(0xFFFFFFFF),
                     {{Color(0xEB4034FF), Color(0xEEEEEEFF)},
                      {Color(0x7700ffFF), Color(0xEEEEEEFF)},
                      {Color(0x00d5ffFF), Color(0xEEEEEEFF)},
                      {Color(0x42f54bFF), Color(0xEEEEEEFF)}},
                     {{Color(0xEB4034FF), Color(0xEEEEEEFF)},
                      {Color(0x7700ffFF), Color(0xEEEEEEFF)},
                      {Color(0x00d5ffFF), Color(0xEEEEEEFF)}}};

  ColorTheme theme = dark;

  Font font;
  font.loadFromFile("assets/calibri.ttf");

  int charSize = 16;

  int explorers = 2;
  Explorer explorer[explorers];

  for (int i = 0; i < explorers; i++) {
    explorer[i] = createExplorer(
        "/home/yorknez/UAIC/IP/Sem1/lab5/SDL_bgi-2.6.0", font, charSize,
        i * WINDOW_W / explorers, 0, WINDOW_W / explorers, WINDOW_H, theme);
  }

  // useful for determining double clicks
  Clock clock;           // a timer that is set between two clicks
  FloatRect clickBounds; // the bounds of the last click
  Input *activeInput = nullptr;
  RectangleShape cursor; // cursor to display on inputs

  while (window.isOpen()) {
    Event event;
    while (window.pollEvent(event)) {
      switch (event.type) {
      case Event::Closed:
        window.close();
        break;
      case Event::KeyPressed:
        // used for scroll
        if (event.key.code == Keyboard::Up) {
          // updateExplorerState()
          explorer[0].scrollOffset -= 50;
          updateFilesY(explorer[0].files,
                       explorer[0].background.getPosition().y + 40 +
                           explorer[0].scrollOffset);
        } else if (event.key.code == Keyboard::Down) {
          explorer[0].scrollOffset += 50;
          updateFilesY(explorer[0].files,
                       explorer[0].background.getPosition().y + 40 +
                           explorer[0].scrollOffset);
        }

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
        for (int i = 0; i < explorers; i++) {
          updateExplorerState(explorer[i], event, RELEASE, clickBounds,
                              activeInput);
        }

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

        // double click
        if (clock.getElapsedTime().asMilliseconds() <= DCLICK_MAX_DELAY) {
          for (int i = 0; i < explorers; i++) {
            updateExplorerState(explorer[i], event, DCLICK, clickBounds,
                                activeInput);
          }
        }
        // simple click
        else {
          for (int i = 0; i < explorers; i++) {
            updateExplorerState(explorer[i], event, CLICK, clickBounds,
                                activeInput);
          }
        }

        // reset timer if a click happened
        clock.restart();

        break;
      case Event::MouseMoved:
        for (int i = 0; i < explorers; i++) {
          updateExplorerState(explorer[i], event, MOVE, clickBounds,
                              activeInput);
        }
        break;
      }
    }

    window.clear(theme.background);
    for (int i = 0; i < explorers; i++) {
      drawExplorer(window, explorer[i]);
    }
    window.display();
  }

  return 0;
}
