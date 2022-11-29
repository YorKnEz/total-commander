#include "dllist.h"
#include "element.h"
#include "filetree.h"
#include "theme.h"
#include <stdio.h>
#include <winbgim.h>
#include <windows.h>

#define WINDOW_W 1280
#define WINDOW_H 720

void initGraphics();

int main() {
  initGraphics();

  Button b = createButton("test12312", WINDOW_W - 120, WINDOW_H - 40,
                          getRGBFromHex(0xEB4034), getRGBFromHex(0xEEEEEE),
                          getRGBFromHex(0xEB4034));

  Button b2 = createButton("alt buton", b.coords.x - b.width - 20, b.coords.y,
                           getRGBFromHex(0xEB4034), getRGBFromHex(0xEEEEEE),
                           getRGBFromHex(0xEB4034));

  drawButton(b);
  drawButton(b2);

  list l;
  init(l);

  string path = "D:\\alex_\\Documents";

  getFilesFromPath(l, path);

  // printList(l);

  node *p = l.head;
  File file;

  file.coords.x = 0;
  file.coords.y = 0;

  file.width = WINDOW_W / 2;
  file.height = 30;

  file.dateColumn = textwidth((char *)"dd/mm/yyyy hh:mm xx");
  file.extColumn = (file.width - file.dateColumn) / 4;
  file.filenameColumn = (file.width - file.dateColumn) / 2;
  file.sizeColumn = (file.width - file.dateColumn) / 4;

  file.textColor = dark.text;
  file.backgroundColor = dark.background;
  file.borderColor = dark.background;

  while (p) {
    file.data = p->data;

    drawFile(file);

    file.coords.y += file.height;
    p = p->next;
  }

  Point mouse;

  while (true) {
    b.oldState = b.state;
    b2.oldState = b2.state;

    if (ismouseclick(WM_LBUTTONDBLCLK)) {
      getmouseclick(WM_LBUTTONDBLCLK, mouse.x, mouse.y);

      b.state = isHovered(b, mouse) ? DCLICKED : INACTIVE;
      b2.state = isHovered(b2, mouse) ? DCLICKED : INACTIVE;
    } else if (ismouseclick(WM_LBUTTONUP)) {
      getmouseclick(WM_LBUTTONUP, mouse.x, mouse.y);

      if (b.state == CLICKED || b.state == DCLICKED) {
        b.state = isHovered(b, mouse) ? HOVERED : INACTIVE;
      }
      if (b2.state == CLICKED || b2.state == DCLICKED) {
        b2.state = isHovered(b2, mouse) ? HOVERED : INACTIVE;
      }
    } else if (ismouseclick(WM_LBUTTONDOWN)) {
      getmouseclick(WM_LBUTTONDOWN, mouse.x, mouse.y);

      b.state = isHovered(b, mouse) ? CLICKED : INACTIVE;
      b2.state = isHovered(b2, mouse) ? CLICKED : INACTIVE;
    } else if (ismouseclick(WM_MOUSEMOVE)) {
      getmouseclick(WM_MOUSEMOVE, mouse.x, mouse.y);

      if (b.state != CLICKED && b.state != DCLICKED) {
        b.state = isHovered(b, mouse) ? HOVERED : INACTIVE;
      }
      if (b2.state != CLICKED && b2.state != DCLICKED) {
        b2.state = isHovered(b2, mouse) ? HOVERED : INACTIVE;
      }
    }

    drawButton(b);
    drawButton(b2);

    Sleep(1000 / 60);
  }

  getch();

  return 0;
}

void initGraphics() {
  // init window
  initwindow(WINDOW_W, WINDOW_H, "Total Commander",
             getmaxwidth() / 2 - WINDOW_W / 2,
             getmaxheight() / 2 - WINDOW_H / 2);

  settextstyle(COMPLEX_FONT, HORIZ_DIR, 0);
  setusercharsize(1, 2, 1, 2);

  setbkcolor(COLOR(dark.background.r, dark.background.g, dark.background.b));
  cleardevice();
}