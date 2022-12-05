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

  Button buttons[11];

  for (int i = 1; i <= 10; i++) {
    buttons[i] =
        createButton("test12312", font, WINDOW_W - i * (btnw + 20),
                     WINDOW_H - 60, btnw, 40, theme.buttonStateColors, 1);
  }

  Clock clock;
  bool click = false;

  // useful for determining double clicks
  Clock clock;           // a timer that is set between two clicks
  bool click = false;    // a flag which checks if we have a double click
  FloatRect clickBounds; // the bounds of the last click

  // list l;
  // init(l);

  // string path = "C:\\Proiect";

  // getFilesFromPath(l, path);

  // // printList(l);

  // node *p = l.head;
  // File file;

  // sortFiletree(l, FILE_DATE, ASC);

  // while (p) {
  //   file.data = p->data;

  //   drawFile(file);

  //   file.coords.y += file.height;
  //   p = p->next;
  // }

  while (window.isOpen()) {
    Event event;
    while (window.pollEvent(event)) {

      switch (event.type) {
      case Event::Closed:
        window.close();
        break;
      case Event::MouseButtonReleased:
        for (int i = 1; i <= 10; i++) {
          updateButtonState(buttons[i], event, RELEASE, clickBounds);
        }
        break;
      case Event::MouseButtonPressed:
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
        break;
      }
    }

    window.clear(theme.background);
    for (int i = 1; i <= 10; i++) {
      drawButton(window, buttons[i]);
    }

    drawText(window, test1);
    window.display();
  }

  return 0;
}