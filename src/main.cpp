// #include "dllist.h"
#include "element.h"
// #include "filetree.h"
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

  while (window.isOpen()) {
    Event event;
    while (window.pollEvent(event)) {
      // b.oldState = b.state;

      switch (event.type) {
      case Event::Closed:
        window.close();
        break;
      case Event::MouseButtonReleased:
        // if (b.state == CLICKED || b.state == DCLICKED) {
        //   b.state = isHovered(b, event.mouseButton.x, event.mouseButton.y)
        //                 ? HOVERED
        //                 : INACTIVE;
        // }
        for (int i = 1; i <= 10; i++) {
          updateButtonState(buttons[i], event, RELEASE);
        }
        break;
      case Event::MouseButtonPressed:
        // double click
        if (click &&
            clock.getElapsedTime().asMilliseconds() <= DCLICK_MAX_DELAY) {
          // b.state = isHovered(b, event.mouseButton.x, event.mouseButton.y)
          //               ? DCLICKED
          //               : INACTIVE;
          for (int i = 1; i <= 10; i++) {
            updateButtonState(buttons[i], event, DCLICK);
          }

          click = false;
        }
        // simple click
        else {
          // b.state = isHovered(b, event.mouseButton.x, event.mouseButton.y)
          //               ? CLICKED
          //               : INACTIVE;
          for (int i = 1; i <= 10; i++) {
            updateButtonState(buttons[i], event, CLICK);
          }

          click = true;
          clock.restart();
        }

        break;
      case Event::MouseMoved:
        // if (b.state != CLICKED && b.state != DCLICKED) {
        //   b.state = isHovered(b, event.mouseMove.x, event.mouseMove.y)
        //                 ? HOVERED
        //                 : INACTIVE;
        // }
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
    window.display();
  }

  // list l;
  // init(l);

  // string path = "D:\\alex_\\Documents";

  // getFilesFromPath(l, path);

  // // printList(l);

  // node *p = l.head;
  // File file;

  // file.coords.x = 0;
  // file.coords.y = 0;

  // file.width = WINDOW_W / 2;
  // file.height = 30;

  // file.dateColumn = textwidth("dd/mm/yyyy hh:mm xx");
  // file.extColumn = (file.width - file.dateColumn) / 4;
  // file.filenameColumn = (file.width - file.dateColumn) / 2;
  // file.sizeColumn = (file.width - file.dateColumn) / 4;

  // file.textColor = dark.text;
  // file.backgroundColor = dark.background;
  // file.borderColor = dark.background;

  // sortFiletree(l, FILE_DATE, ASC);

  // while (p) {
  //   file.data = p->data;

  //   drawFile(file);

  //   file.coords.y += file.height;
  //   p = p->next;
  // }

  // Point mouse;

  // while (true) {
  //   b.oldState = b.state;
  //   b2.oldState = b2.state;

  //   if (ismouseclick(WM_LBUTTONDBLCLK)) {
  //     getmouseclick(WM_LBUTTONDBLCLK, mouse.x, mouse.y);

  //     b.state = isHovered(b, mouse) ? DCLICKED : INACTIVE;
  //     b2.state = isHovered(b2, mouse) ? DCLICKED : INACTIVE;
  //   } else if (ismouseclick(WM_LBUTTONUP)) {
  //     getmouseclick(WM_LBUTTONUP, mouse.x, mouse.y);

  //     if (b.state == CLICKED || b.state == DCLICKED) {
  //       b.state = isHovered(b, mouse) ? HOVERED : INACTIVE;
  //     }
  //     if (b2.state == CLICKED || b2.state == DCLICKED) {
  //       b2.state = isHovered(b2, mouse) ? HOVERED : INACTIVE;
  //     }
  //   } else if (ismouseclick(WM_LBUTTONDOWN)) {
  //     getmouseclick(WM_LBUTTONDOWN, mouse.x, mouse.y);

  //     b.state = isHovered(b, mouse) ? CLICKED : INACTIVE;
  //     b2.state = isHovered(b2, mouse) ? CLICKED : INACTIVE;
  //   } else if (ismouseclick(WM_MOUSEMOVE)) {
  //     getmouseclick(WM_MOUSEMOVE, mouse.x, mouse.y);

  //     if (b.state != CLICKED && b.state != DCLICKED) {
  //       b.state = isHovered(b, mouse) ? HOVERED : INACTIVE;
  //     }
  //     if (b2.state != CLICKED && b2.state != DCLICKED) {
  //       b2.state = isHovered(b2, mouse) ? HOVERED : INACTIVE;
  //     }
  //   }

  //   drawButton(b);
  //   drawButton(b2);

  //   Sleep(1000 / 60);
  // }

  // getch();

  return 0;
}