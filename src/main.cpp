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

#define DCLICK_MAX_DELAY 500

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

  Text test1 = createText("test pentru text", font, 30, 200, 200, 300,
                          Color(0xEF3030FF));

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
          updateButtonState(buttons[i], event, RELEASE);
        }
        break;
      case Event::MouseButtonPressed:
        // double click
        if (click &&
            clock.getElapsedTime().asMilliseconds() <= DCLICK_MAX_DELAY) {
          for (int i = 1; i <= 10; i++) {
            updateButtonState(buttons[i], event, DCLICK);
          }

          click = false;
        }
        // simple click
        else {
          for (int i = 1; i <= 10; i++) {
            updateButtonState(buttons[i], event, CLICK);
          }
          click = true;
          clock.restart();
        }

        break;
      case Event::MouseMoved:
        for (int i = 1; i <= 10; i++) {
          updateButtonState(buttons[i], event, MOVE);
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