#include <winbgim.h>

#define WINDOW_W 1280
#define WINDOW_H 720

void initGraphics();

int main() {
  initGraphics();

  getch();

  return 0;
}

void initGraphics() {
  int driver = DETECT, mode;

  // init graphics mode
  initgraph(&driver, &mode, "");

  // init window
  initwindow(WINDOW_W, WINDOW_H, "Total Commander",
             getmaxwidth() / 2 - WINDOW_W / 2,
             getmaxheight() / 2 - WINDOW_H / 2);
}