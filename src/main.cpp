#include "element.h"
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
  // center window on the screen
  window.setPosition(
      Vector2i(VideoMode::getDesktopMode().width / 2 - WINDOW_W / 2,
               VideoMode::getDesktopMode().height / 2 - WINDOW_H / 2));

  ColorTheme dark = {
      Color(0xFFFFFFFF), // text with high contrast
      Color(0x999995FF), // text with medium contrast
      Color(0x5B5C55FF), // text with low contrast
      Color(0x32332BFF), // background of body
      Color(0x191A16FF), // background with low contrast
      Color(0x0A0A09FF), // border
      {{Color(0x848580FF), Color(0x282922FF), Color(0x0A0A09FF)},
       {Color(0xC2C2BFFF), Color(0x282922FF), Color(0x191A16FF)},
       {Color(0xFFFFFFFF), Color(0x282922FF), Color(0x0A0A09FF)},
       {Color(0xFFFFFFFF), Color(0x282922FF), Color(0x0A0A09FF)}},
      {{Color(0x999995FF), Color(0x5B5C55FF), Color(0x32332BFF),
        Color(0x0A0A09FF)},
       {Color(0x999995FF), Color(0x5B5C55FF), Color(0x00FF0032),
        Color(0x0A0A09FF)},
       {Color(0x999995FF), Color(0x5B5C55FF), Color(0x0000FF32),
        Color(0x0A0A09FF)}},
      {{Color(0x848580FF), Color(0x191A16FF), Color(0x0A0A09FF)},
       {Color(0xC2C2BFFF), Color(0x191A16FF), Color(0x191A16FF)},
       {Color(0xFFFFFFFF), Color(0x191A16FF), Color(0x0A0A09FF)}}};

  ColorTheme yellow = {
      Color(0xffffffff), // text with high contrast
      Color(0xfedd81ff), // text with medium contrast
      Color(0xfdc835ff), // text with low contrast
      Color(0xfcba03ff), // background of body
      Color(0x7e5d02ff), // background with low contrast
      Color(0x322501ff), // border
      {{Color(0xfdd668ff), Color(0xca9502ff), Color(0x322501ff)},
       {Color(0x4c3801ff), Color(0xca9502ff), Color(0x7e5d02ff)},
       {Color(0xffffffff), Color(0xca9502ff), Color(0x322501ff)},
       {Color(0xffffffff), Color(0xca9502ff), Color(0x322501ff)}},
      {{Color(0xfedd81ff), Color(0xca9502ff), Color(0xfcba03),
        Color(0x322501ff)},
       {Color(0xfedd81ff), Color(0xca9502ff), Color(0x00FF0032),
        Color(0x322501ff)},
       {Color(0xfedd81ff), Color(0xca9502ff), Color(0x0000FF32),
        Color(0x322501ff)}},
      {{Color(0xfdd668ff), Color(0x7e5d02ff), Color(0x322501ff)},
       {Color(0x4c3801ff), Color(0x7e5d02ff), Color(0x7e5d02ff)},
       {Color(0xffffffff), Color(0x7e5d02ff), Color(0x322501ff)}}};

  ColorTheme green = {
     Color(0xffffffff), // text with high contrast
      Color(0x8dbd96ff), // text with medium contrast
      Color(0x499556ff), // text with low contrast
      Color(0x1b7a2cff), // background of body
      Color(0x0e3d16ff), // background with low contrast
      Color(0x051809ff), // border
      {{Color(0x76af80ff), Color(0x166223ff), Color(0x051809ff)},
       {Color(0xbbd7c0ff), Color(0x166223ff), Color(0x0e3d16ff)},
       {Color(0xffffffff), Color(0x166223ff), Color(0x051809ff)},
       {Color(0xffffffff), Color(0x166223ff), Color(0x051809ff)}},
      {{Color(0x8dbd96ff), Color(0x499556ff), Color(0x1b7a2cff),
        Color(0x051809ff)},
       {Color(0x8dbd96ff), Color(0x499556ff), Color(0xFF000032),
        Color(0x051809ff)},
       {Color(0x8dbd96ff), Color(0x499556ff), Color(0x0000FF32),
        Color(0x051809ff)}},
      {{Color(0x76af80ff), Color(0x0e3d16ff), Color(0x051809ff)},
       {Color(0xbbd7c0ff), Color(0x0e3d16ff), Color(0x0e3d16ff)},
       {Color(0xffffffff), Color(0x0e3d16ff), Color(0x051809ff)}}};

  ColorTheme theme = dark;

  Font font;
  font.loadFromFile("assets/hack.ttf");

  int charSize = 16;

  int buttons = 6;
  int btnWidth = WINDOW_W / buttons, btnHeight = 32;
  Button button[buttons];
  string buttonString[buttons] = {"Rename", "Edit",  "Copy",
                                  "Move",   "Mkdir", "Delete"};

  for (int i = 0; i < buttons; i++) {
    button[i] = createButton(buttonString[i], font, charSize, i * btnWidth + 2,
                             WINDOW_H - btnHeight + 1, btnWidth - 2,
                             btnHeight - 2, theme.buttonStateColors, 1);
  }

  int explorers = 2;
  Explorer explorer[explorers];

  for (int i = 0; i < explorers; i++) {
    explorer[i] =
        createExplorer(getDefaultPath(), font, charSize,
                       i * WINDOW_W / explorers, 0, WINDOW_W / explorers,
                       WINDOW_H - btnHeight, theme);
  }

  // useful for determining double clicks
  Clock clock;           // a timer that is set between two clicks
  FloatRect clickBounds; // the bounds of the last click
  Input *activeInput = nullptr;
  Explorer *activeExplorer =
      nullptr; // current active explorer will be the first one by default
  RectangleShape cursor; // cursor to display on inputs

  while (window.isOpen()) {
    Event event;
    while (window.pollEvent(event)) {
      switch (event.type) {
      case Event::Closed:
        window.close();
        break;
      case Event::MouseWheelScrolled:
        if (activeExplorer) {
          if (event.mouseWheelScroll.delta < 0) {
            // updateExplorerState()
            activeExplorer->scrollOffset -= 50;
            updateFilesY(activeExplorer->files,
                         activeExplorer->background.getPosition().y +
                             activeExplorer->heightFile +
                             2 * activeExplorer->heightComp +
                             activeExplorer->scrollOffset);
          } else if (event.mouseWheelScroll.delta > 0) {
            activeExplorer->scrollOffset += 50;
            updateFilesY(activeExplorer->files,
                         activeExplorer->background.getPosition().y +
                             activeExplorer->heightFile +
                             2 * activeExplorer->heightComp +
                             activeExplorer->scrollOffset);
          }
        }
        break;
      case Event::KeyPressed:
        // used for scroll
        if (activeExplorer) {
          if (event.key.code == Keyboard::Up) {
            // updateExplorerState()
            activeExplorer->scrollOffset -= 50;
            updateFilesY(activeExplorer->files,
                         activeExplorer->background.getPosition().y +
                             activeExplorer->heightFile +
                             2 * activeExplorer->heightComp +
                             activeExplorer->scrollOffset);
          } else if (event.key.code == Keyboard::Down) {
            activeExplorer->scrollOffset += 50;
            updateFilesY(activeExplorer->files,
                         activeExplorer->background.getPosition().y +
                             activeExplorer->heightFile +
                             2 * activeExplorer->heightComp +
                             activeExplorer->scrollOffset);
          }
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
          updateExplorerState(explorer[i], event, RELEASE, activeExplorer,
                              clickBounds, activeInput, font);
        }

        for (int i = 0; i < buttons; i++) {
          updateButtonState(button[i], event, RELEASE, clickBounds);
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
            updateExplorerState(explorer[i], event, DCLICK, activeExplorer,
                                clickBounds, activeInput, font, theme);
          }

          for (int i = 0; i < buttons; i++) {
            updateButtonState(button[i], event, DCLICK, clickBounds);
          }
        }
        // simple click
        else {
          for (int i = 0; i < explorers; i++) {
            updateExplorerState(explorer[i], event, CLICK, activeExplorer,
                                clickBounds, activeInput, font);
          }

          for (int i = 0; i < buttons; i++) {
            updateButtonState(button[i], event, CLICK, clickBounds);
          }
        }

        // reset timer if a click happened
        clock.restart();

        break;
      case Event::MouseMoved:
        for (int i = 0; i < explorers; i++) {
          updateExplorerState(explorer[i], event, MOVE, activeExplorer,
                              clickBounds, activeInput, font);
        }

        for (int i = 0; i < buttons; i++) {
          updateButtonState(button[i], event, MOVE, clickBounds);
        }
        break;
      }
    }

    window.clear(theme.bgBody);
    for (int i = 0; i < explorers; i++) {
      drawExplorer(window, explorer[i]);
    }

    for (int i = 0; i < buttons; i++) {
      drawButton(window, button[i]);
    }

    drawCursor(window, activeInput);
    window.display();
  }

  return 0;
}
