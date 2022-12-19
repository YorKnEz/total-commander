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

#define E_MAX_STATES 2

enum ExplorerState { E_INACTIVE, E_ACTIVE };
enum Direction { UP = 50, DOWN = -50, DRAG };

struct Explorer {
  string path;
  list files;
  sortBy sortedBy = FILE_NAME;
  sortOrder order = ASC;
  File *activeFile[2] = { nullptr, nullptr };
  int scrollOffset;
  int heightFile; // height of the files on the screen
  int heightComp; // height of the other components
  RectangleShape background;
  Button button[4];
  Input input;
  TextBox textbox[2];
  ExplorerState state;
  Scrollbar scrollbar;
};


Explorer createExplorer(string path, Font &font, int charSize, int x, int y,
                        int width, int height, ColorTheme theme);

void updateScrollbarState(Explorer &explorer, Event event,
                          MouseEventType type, Vector2i &oldClick);

void updateExplorerState(Explorer &explorer, Event event, MouseEventType type,
                         Explorer *&activeExplorer, Vector2i &oldClick,
                         Input *&activeInput, Font &font, ColorTheme theme = {});

void updateFilesY(list &files, int y);

void scrollFiles(Explorer *activeExplorer, Direction d);

void drawExplorer(RenderWindow &window, Explorer explorer);

#endif
