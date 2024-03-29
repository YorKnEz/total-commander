#include "button.h"
#include "element.h"
#include "input.h"

#include "explorer.h"
#include "filetree.h"
#include "menu.h"
#include "settings.h"
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

  // get the current settings
  Settings settings;
  loadSettings(settings);

  // get the current themes
  List<Theme> themes;
  loadThemes(themes);

  // load the default theme
  Node<Theme> *theme = themes.head;

  while (theme->data.name != settings.defaultTheme &&
         theme->next != themes.head) {
    theme = theme->next;
  }

  if (themes.tail->data.name == settings.defaultTheme) {
    theme = themes.tail;
  }

  int buttons = 5;
  int btnWidth = WINDOW_W / buttons, btnHeight = 32;
  Button button[buttons];
  string buttonString[buttons] = {"F2 Rename", "F3 Mkdir", "F4 Copy",
                                  "F5 Delete", "F6 Move"};

  for (int i = 0; i < buttons; i++) {
    button[i] =
        createButton(buttonString[i], theme->data.font, theme->data.charSize,
                     i * btnWidth + 1, WINDOW_H - btnHeight + 1, btnWidth - 2,
                     btnHeight - 2, theme->data.colors.buttonStateColors, 1);
  }

  int explorers = settings.explorers;
  Explorer explorer[explorers];

  for (int i = 0; i < explorers - 1; i++) {
    explorer[i] =
        createExplorer(getDefaultPath(), i * (WINDOW_W / explorers), 0,
                       WINDOW_W / explorers, WINDOW_H - btnHeight, theme->data);
  }

  explorer[explorers - 1] =
      createExplorer(getDefaultPath(), (explorers - 1) * (WINDOW_W / explorers),
                     0, WINDOW_W - (explorers - 1) * (WINDOW_W / explorers),
                     WINDOW_H - btnHeight, theme->data);

  // useful for determining double clicks
  Clock clock; // a timer that is set between two clicks
  Input *activeInput = nullptr;
  Explorer *activeExplorer =
      nullptr; // current active explorer will be the first one by default
  RectangleShape cursor; // cursor to display on inputs
  Vector2i oldClick;     // required for dragging the scrollbar
  string clipboardPath;
  bool cut = false;
  List<File> clipboard;
  clipboard.init();

  while (window.isOpen()) {
    Event event;
    while (window.pollEvent(event)) {
      switch (event.type) {
      case Event::Closed:
        for (int i = 0; i < explorers; i++) {
          closeExplorer(explorer[i]);
        }

        // break the theme cycle
        themes.head->prev = nullptr;
        themes.tail->next = nullptr;

        themes.free();
        clipboard.free();

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
      case Event::KeyPressed: {
        Keyboard kbd; // used for checking key combos

        switch (event.key.code) {
        case Keyboard::A:
          if (activeExplorer && !activeExplorer->forestView &&
              !activeExplorer->search && kbd.isKeyPressed(Keyboard::LControl)) {
            Node<File> *p = activeExplorer->files.head->next;

            while (p) {
              p->data.state = F_SELECTED;

              p = p->next;
            }
          }
          break;
        case Keyboard::X:
          if (activeExplorer && !activeExplorer->forestView &&
              !activeExplorer->search && kbd.isKeyPressed(Keyboard::LControl)) {
            cut = true;
          }
        case Keyboard::C:
          // copy currently selected files into the clipboard
          if (activeExplorer && !activeExplorer->forestView &&
              !activeExplorer->search && kbd.isKeyPressed(Keyboard::LControl)) {
            clipboard.free();
            clipboard.init();
            clipboardPath = activeExplorer->path;

            Node<File> *p = activeExplorer->files.head;

            while (p) {
              if (p->data.state == F_SELECTED &&
                  p->data.data.filename.compare("..")) {
                clipboard.add(p->data, clipboard.length);
              }

              p = p->next;
            }
          }
          break;
        case Keyboard::D:
          if (activeExplorer && !activeExplorer->forestView &&
              !activeExplorer->search && kbd.isKeyPressed(Keyboard::LControl)) {
            Node<File> *p = activeExplorer->files.head; // file list iterator

            while (p) {
              p->data.state = F_INACTIVE;

              p = p->next;
            }
          }
          break;
        case Keyboard::I:
          if (activeExplorer && !activeExplorer->forestView &&
              !activeExplorer->search && kbd.isKeyPressed(Keyboard::LControl)) {
            Node<File> *p =
                activeExplorer->files.head->next; // file list iterator

            while (p) {
              p->data.state =
                  (p->data.state == F_SELECTED) ? F_INACTIVE : F_SELECTED;

              p = p->next;
            }
          }
          break;
        case Keyboard::R:
          // refresh current explorer
          if (activeExplorer && kbd.isKeyPressed(Keyboard::LControl)) {
            refreshExplorer(*activeExplorer, activeExplorer, theme->data);
          }
          break;
        case Keyboard::T:
          if (kbd.isKeyPressed(Keyboard::LControl)) {
            theme = (kbd.isKeyPressed(Keyboard::LShift)) ? theme->prev
                                                         : theme->next;

            for (int i = 0; i < explorers; i++) {
              updateExplorerTheme(explorer[i], theme->data);
            }

            for (int i = 0; i < buttons; i++) {
              updateButtonTheme(button[i],
                                theme->data.colors.buttonStateColors);
            }
          }
          break;
        case Keyboard::V:
          // paste the clipboard's contents into the currently active explorer
          if (clipboard.length > 0 && activeExplorer &&
              !activeExplorer->forestView && !activeExplorer->search &&
              kbd.isKeyPressed(Keyboard::LControl)) {
            Node<File> *p = clipboard.head;

            while (p) {
              string currentEntryName = p->data.data.filename;
              string currentEntryExt = p->data.data.ext;
              string currentEntry = currentEntryName;

              // add the extension to the crt entry if it exists
              if (!currentEntryExt.empty()) {
                currentEntry += "." + currentEntryExt;
              }

              if (cut) {
                moveEntry(clipboardPath + SEP + currentEntry,
                          activeExplorer->path);
              } else {
                copyEntry(clipboardPath + SEP + currentEntry,
                          activeExplorer->path);
              }

              p = p->next;
            }

            cut = false;

            for (int i = 0; i < explorers; i++) {
              if (explorer[i].path == clipboardPath ||
                  explorer[i].path == activeExplorer->path) {
                refreshExplorer(explorer[i], activeExplorer, theme->data);
              }
            }
          }
          break;
        case Keyboard::Enter: {
          // submit input
          if (activeExplorer) {
            if (activeInput) {
              if (activeExplorer->search) {
                // get current head of the file to exctract it's props
                FloatRect fileBounds = activeExplorer->files.head->data
                                           .background.getGlobalBounds();

                List<File> results;
                results.init();

                searchFile(results, activeExplorer->path, activeInput->value,
                           theme->data, fileBounds.left, fileBounds.top,
                           fileBounds.width, fileBounds.height);

                if (results.length > 0) {
                  // copy the results inside the list
                  activeExplorer->files.free();
                  activeExplorer->files.init();

                  Node<File> *p = results.head;

                  while (p) {
                    activeExplorer->files.add(p->data, 0);
                    p = p->next;
                  }

                  results.free();
                }

                refreshExplorer(*activeExplorer, activeExplorer, theme->data);
              } else {
                string destinationPath = evalPath(activeInput->value);

                if (fs::is_directory(destinationPath)) {
                  activeExplorer->path = destinationPath;
                  refreshExplorer(*activeExplorer, activeExplorer, theme->data);
                } else {
                  createErrorPopUp(POP_UP_DEFAULT_W, POP_UP_DEFAULT_H, "Error",
                                   "The given path is invalid.", theme->data);
                }
              }
            }
            // open currently selected file
            else if (!activeExplorer->forestView &&
                     activeExplorer->activeFile[0]) {
              string currentEntryName =
                  activeExplorer->activeFile[0]->data.data.filename;
              string currentEntryExt =
                  activeExplorer->activeFile[0]->data.data.ext;
              string currentEntry = currentEntryName;

              // add the extension to the crt entry if it exists
              if (!currentEntryExt.empty()) {
                currentEntry += "." + currentEntryExt;
              }

              string path =
                  activeExplorer->activeFile[0]->data.data.path.size() > 0
                      ? activeExplorer->activeFile[0]->data.data.path
                      : activeExplorer->path;

              openEntry(path, currentEntryName, currentEntryExt);

              // refresh the explorer if the user opened a folder
              if (activeExplorer->activeFile[0]->data.data.size == "<DIR>") {
                activeExplorer->path = path;

                refreshExplorer(*activeExplorer, activeExplorer, theme->data);
              }
            }
          }
          break;
        }
        case Keyboard::Backspace:
          if (activeInput) {
            eraseChar(activeInput);
          }
          // go back one folder
          else if (activeExplorer && !activeExplorer->forestView &&
                   !activeExplorer->search) {
            openFolder(activeExplorer->path, "..");

            refreshExplorer(*activeExplorer, activeExplorer, theme->data);
          }
          break;
        case Keyboard::Tab:
          if (activeExplorer) {
            int increment = kbd.isKeyPressed(Keyboard::LShift) ? -1 : 1;

            // disable the old active input
            if (activeInput) {
              activeInput->state = I_INACTIVE;
              activeInput = nullptr;
            }

            // move the active indicator to the next explorer
            updateExplorerIndicator(
                &explorer[(explorers + (activeExplorer - explorer) +
                           increment) %
                          explorers],
                activeExplorer);
          }
          break;
        case Keyboard::Delete:
          if (activeExplorer && activeExplorer->activeFile[0] &&
              !activeExplorer->forestView && !activeExplorer->search) {
            handleMenuButtons(explorer, explorers, activeExplorer, DELETE_ENTRY,
                              TITLE, theme->data);
          }

          break;
        case Keyboard::Left:
          if (activeInput) {
            moveCursor(activeInput, -1);
          }
          break;
        case Keyboard::Right:
          if (activeInput) {
            moveCursor(activeInput, 1);
          }
          break;
        case Keyboard::Up:
          if (activeExplorer) {
            if (activeExplorer->activeFile[0]) {
              Node<File> *p = activeExplorer->files.head; // file list iterator

              while (p) {
                p->data.state = F_INACTIVE;

                p = p->next;
              }
              if (activeExplorer->activeFile[0]->prev) {
                activeExplorer->activeFile[0]->data.state = F_INACTIVE;
                activeExplorer->activeFile[0] =
                    activeExplorer->activeFile[0]->prev;
              }
              activeExplorer->activeFile[0]->data.state = F_SELECTED;
              // get  bounds of the area in which the files are displayed
              int displayStartY =
                  2 * activeExplorer->heightComp + activeExplorer->heightFile;

              // get the bounds of the currently active file
              FloatRect fileBounds = activeExplorer->activeFile[0]
                                         ->data.background.getGlobalBounds();

              // calculate offset for scroll
              int offset = displayStartY - fileBounds.top;

              // scroll if the currently active file is outside the screen
              if (offset > 0) {
                scrollFiles(activeExplorer, Direction(offset));
              }
            } else {
              scrollFiles(activeExplorer, UP);
            }
          }
          break;
        case Keyboard::Down:
          if (activeExplorer) {
            if (activeExplorer->activeFile[0]) {
              Node<File> *p = activeExplorer->files.tail; // file list iterator

              // deselect all files
              while (p) {
                p->data.state = F_INACTIVE;

                p = p->prev;
              }

              if (activeExplorer->activeFile[0]->next) {
                // move to the next file
                activeExplorer->activeFile[0] =
                    activeExplorer->activeFile[0]->next;
              }
              activeExplorer->activeFile[0]->data.state = F_SELECTED;
              // get  bounds of the area in which the files are displayed
              int displayStartY =
                  2 * activeExplorer->heightComp + activeExplorer->heightFile;
              int displayHeight =
                  activeExplorer->background.getGlobalBounds().height -
                  3 * activeExplorer->heightComp - activeExplorer->heightFile;

              // get the bounds of the currently active file
              FloatRect fileBounds = activeExplorer->activeFile[0]
                                         ->data.background.getGlobalBounds();

              // calculate offset for scroll
              int offset = displayStartY + displayHeight - fileBounds.top - 1;

              // scroll if the currently active file is outside the screen
              if (fileBounds.height > offset) {
                scrollFiles(activeExplorer,
                            Direction(offset - fileBounds.height));
              }
            } else {
              scrollFiles(activeExplorer, DOWN);
            }
          }
          break;
        case Keyboard::F1: {
          string projectLink = "https://github.com/YorKnEz/total-commander";
          string command = SEP == "\\" ? "start \"\" " + projectLink
                                       : "xdg-open " + projectLink;

          system(command.c_str());
          break;
        }
        case Keyboard::F2:
        case Keyboard::F3:
        case Keyboard::F4:
        case Keyboard::F5:
          // sorting shortcuts
          if (activeExplorer && kbd.isKeyPressed(Keyboard::LShift)) {
            sortFiles(*activeExplorer, sortBy(event.key.code - Keyboard::F2),
                      &theme->data.upArrow, &theme->data.downArrow);
            break;
          }
        case Keyboard::F6:
          // menu buttons shortcuts
          // they get triggered only when in normal view
          if (activeExplorer && !activeExplorer->forestView &&
              !activeExplorer->search) {
            handleMenuButtons(explorer, explorers, activeExplorer,
                              MenuButtons(event.key.code - Keyboard::F2), TITLE,
                              theme->data);
          }
          break;
        }
        break;
      }
      case Event::TextEntered:
        if (activeInput) {
          char enteredChar = event.text.unicode;

          // normal characters
          if (31 < enteredChar && enteredChar < 128) {
            insertChar(activeInput, enteredChar);
          }
        }
        break;
      case Event::MouseButtonReleased:
        for (int i = 0; i < explorers; i++) {
          updateExplorerState(explorer[i], event, RELEASE, activeExplorer,
                              oldClick, activeInput, theme->data);
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

          if (button[i].state == B_CLICKED && activeExplorer &&
              !activeExplorer->forestView && !activeExplorer->search) {
            handleMenuButtons(explorer, explorers, activeExplorer,
                              MenuButtons(i), TITLE, theme->data);
          }
        }

        // double click
        if (clock.getElapsedTime().asMilliseconds() <= DCLICK_MAX_DELAY) {
          for (int i = 0; i < explorers; i++) {
            updateExplorerState(explorer[i], event, DCLICK, activeExplorer,
                                oldClick, activeInput, theme->data);
          }

        }
        // simple click
        else {
          for (int i = 0; i < explorers; i++) {
            updateExplorerState(explorer[i], event, CLICK, activeExplorer,
                                oldClick, activeInput, theme->data);
          }
        }

        // reset timer if a click happened
        clock.restart();

        break;
      case Event::MouseMoved:
        for (int i = 0; i < explorers; i++) {
          updateExplorerState(explorer[i], event, MOVE, activeExplorer,
                              oldClick, activeInput, theme->data);
        }

        for (int i = 0; i < buttons; i++) {
          updateButtonState(button[i], event, MOVE, oldClick);
        }

        break;
      }
    }

    window.clear(theme->data.colors.bgBody);
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
