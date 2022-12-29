#include "button.h"
#include "element.h"
#include "input.h"

#include "explorer.h"
#include "filetree.h"
#include "menu.h"
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
  string buttonString[buttons] = {"Open",   "Mkdir", "Copy",
                                  "Delete", "Move",  "Rename"};
  string newName, newPath, currentEntryName, currentEntryExt, currentEntry;
  enum buttons {
    OPEN_ENTRY,
    MKDIR,
    COPY_ENTRY,
    DELETE_ENTRY,
    MOVE_ENTRY,
    RENAME_ENTRY
  };

  for (int i = 0; i < buttons; i++) {
    button[i] = createButton(buttonString[i], font, charSize, i * btnWidth + 2,
                             WINDOW_H - btnHeight + 1, btnWidth - 2,
                             btnHeight - 2, theme.buttonStateColors, 1);
  }

  int explorers = 2;
  Explorer explorer[explorers];

  for (int i = 0; i < explorers; i++) {
    explorer[i] = createExplorer(
        getDefaultPath(), font, charSize, i * WINDOW_W / explorers, 0,
        WINDOW_W / explorers, WINDOW_H - btnHeight, theme);
  }

  // useful for determining double clicks
  Clock clock; // a timer that is set between two clicks
  Input *activeInput = nullptr;
  Explorer *activeExplorer =
      nullptr; // current active explorer will be the first one by default
  RectangleShape cursor; // cursor to display on inputs
  Vector2i oldClick;     // required for dragging the scrollbar

  while (window.isOpen()) {
    Event event;
    while (window.pollEvent(event)) {
      switch (event.type) {
      case Event::Closed:
        for (int i = 0; i < explorers; i++) {
          closeExplorer(explorer[i]);
        }

        window.close();
        break;
      case Event::MouseWheelScrolled:
        if (activeExplorer) {
          // scroll up
          if (event.mouseWheelScroll.delta > 0) {
            scrollFiles(activeExplorer, UP);
          }
          // scroll down
          else if (event.mouseWheelScroll.delta < 0) {
            scrollFiles(activeExplorer, DOWN);
          }
        }
        break;
      case Event::KeyPressed:
        if (activeExplorer) {
          // scroll up
          if (event.key.code == Keyboard::Up) {
            scrollFiles(activeExplorer, UP);
          }
          // scroll down
          else if (event.key.code == Keyboard::Down) {
            scrollFiles(activeExplorer, DOWN);
          }
        }

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
            activeExplorer->path = activeInput->value;
            string filename = activeInput->value.substr(
                activeInput->value.find_last_of(SEP) + 1);

            // add an error pop up
            openFolder(activeExplorer->path, filename);

            refreshExplorer(*activeExplorer, activeExplorer, font, theme);
          }
          // normal characters
          else if (31 < enteredChar && enteredChar < 128) {
            insertChar(activeInput, enteredChar);
          }
        }
        break;
      case Event::MouseButtonReleased:
        for (int i = 0; i < explorers; i++) {
          updateExplorerState(explorer[i], event, RELEASE, activeExplorer,
                              oldClick, activeInput, font);
        }

        for (int i = 0; i < buttons; i++) {
          updateButtonState(button[i], event, RELEASE, oldClick);
        }

        break;
      case Event::MouseButtonPressed:
        oldClick = Vector2i(event.mouseButton.x, event.mouseButton.y);
        // disable the last active input if user clicks outside it's box,
        // regardless of type of click
        if (activeInput &&
            !isHovered(activeInput->background.getGlobalBounds(),
                       event.mouseButton.x, event.mouseButton.y)) {
          activeInput->state = I_INACTIVE;
          activeInput = nullptr;
        }

        for (int i = 0; i < buttons; i++) {
          updateButtonState(button[i], event, CLICK, oldClick);

          string newPath = "";
          string newName, currentEntryName, currentEntryExt, currentEntry;

          if (button[i].state == B_CLICKED && activeExplorer) {
            switch (i) {
            case MKDIR:
              newName = createPopUp(POP_UP_DEFAULT_W, POP_UP_DEFAULT_H,
                                    GET_FILENAME, TITLE, "Folder name: ", "OK",
                                    "Folder name", "", font, charSize, theme);

              if (!newName.empty()) {
                createFolder(activeExplorer->path, newName);
              }

              break;
            case OPEN_ENTRY:
              if (activeExplorer->activeFile[0]) {
                currentEntryName =
                    activeExplorer->activeFile[0]->data.data.filename;
                currentEntryExt = activeExplorer->activeFile[0]->data.data.ext;
                currentEntry = currentEntryName;

                // add the extension to the crt entry if it exists
                if (!currentEntryExt.empty()) {
                  currentEntry += "." + currentEntryExt;
                }

                openEntry(activeExplorer->path, currentEntryName,
                          currentEntryExt);
              }

              break;
            case COPY_ENTRY:
              if (activeExplorer->activeFile[0]) {
                newPath =
                    createPopUp(POP_UP_DEFAULT_W, POP_UP_DEFAULT_H, GET_PATH,
                                TITLE, "Copy to:", "OK", "Destination path",
                                activeExplorer->path, font, charSize, theme);

                if (!newPath.empty()) {
                  node *p = activeExplorer->files.head;

                  while (p) {
                    if (p->data.state == F_SELECTED && p->data.data.filename.compare("..")) {
                      currentEntryName = p->data.data.filename;
                      currentEntryExt = p->data.data.ext;
                      currentEntry = currentEntryName;

                      // add the extension to the crt entry if it exists
                      if (!currentEntryExt.empty()) {
                        currentEntry += "." + currentEntryExt;
                      }

                      copyEntry(activeExplorer->path + SEP + currentEntry,
                                newPath);
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
                  if (p->data.state == F_SELECTED && p->data.data.filename.compare("..")) {
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
                newPath =
                    createPopUp(POP_UP_DEFAULT_W, POP_UP_DEFAULT_H, GET_PATH,
                                TITLE, "Move to:", "OK", "Destination path",
                                activeExplorer->path, font, charSize, theme);

                if (!newPath.empty()) {
                  node *p = activeExplorer->files.head;

                  while (p) {
                    if (p->data.state == F_SELECTED && p->data.data.filename.compare("..")) {
                      currentEntryName = p->data.data.filename;
                      currentEntryExt = p->data.data.ext;
                      currentEntry = currentEntryName;

                      // add the extension to the crt entry if it exists
                      if (!currentEntryExt.empty()) {
                        currentEntry += "." + currentEntryExt;
                      }

                      moveEntry(activeExplorer->path + SEP + currentEntry,
                                newPath);
                    }

                    p = p->next;
                  }
                }
              }

              break;
            case RENAME_ENTRY:
              if (activeExplorer->activeFile[0]) {

                newName = createPopUp(POP_UP_DEFAULT_W, POP_UP_DEFAULT_H,
                                      GET_FILENAME, TITLE, "Rename to: ", "OK",
                                      "New name", "", font, charSize, theme);

                if (!newName.empty()) {
                  node *p = activeExplorer->files.head;

                  while (p) {
                    if (p->data.state == F_SELECTED && p->data.data.filename.compare("..")) {
                      currentEntryName = p->data.data.filename;
                      currentEntryExt = p->data.data.ext;
                      currentEntry = currentEntryName;

                      // add the extension to the crt entry if it exists
                      if (!currentEntryExt.empty()) {
                        currentEntry += "." + currentEntryExt;
                      }

                      editEntryName(activeExplorer->path + SEP + currentEntry,
                                    newName);
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
        }

        // double click
        if (clock.getElapsedTime().asMilliseconds() <= DCLICK_MAX_DELAY) {
          for (int i = 0; i < explorers; i++) {
            updateExplorerState(explorer[i], event, DCLICK, activeExplorer,
                                oldClick, activeInput, font, theme);
          }

        }
        // simple click
        else {
          for (int i = 0; i < explorers; i++) {
            updateExplorerState(explorer[i], event, CLICK, activeExplorer,
                                oldClick, activeInput, font);
          }
        }

        // reset timer if a click happened
        clock.restart();

        break;
      case Event::MouseMoved:
        for (int i = 0; i < explorers; i++) {
          updateExplorerState(explorer[i], event, MOVE, activeExplorer,
                              oldClick, activeInput, font);
        }

        for (int i = 0; i < buttons; i++) {
          updateButtonState(button[i], event, MOVE, oldClick);
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
