#ifndef EXPLORER_H
#define EXPLORER_H

#include "dllist.h"
#include "element.h"
#include "filetree.h"
#include "theme.h"
#include <SFML/Graphics.hpp>
#include <string>

using namespace sf;
using namespace std;

struct Explorer {
  string path;
  list files;
  int scrollOffset;
  RectangleShape background;
  Button button[4];
  Input input;
  TextBox textbox[2];
};

Explorer createExplorer(string path, Font &font, int charSize, int x, int y,
                        int width, int height, ColorTheme theme);

void updateExplorerState(Explorer &explorer, Event event, MouseEventType type,
void updateFilesY(list &files, int y);

void drawExplorer(RenderWindow &window, Explorer explorer);

#endif
