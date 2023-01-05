#ifndef EXPLORER_H
#define EXPLORER_H

#include "button.h"
#include "element.h"
#include "input.h"
#include "scrollbar.h"
#include "textbox.h"

#include "dllist.h"
#include "filetree.h"
#include "theme.h"
#include <SFML/Graphics.hpp>
#include <string>

using namespace sf;
using namespace std;

#define E_MAX_STATES 2

enum ExplorerState { E_INACTIVE, E_ACTIVE };
enum Direction { UP = 50, DOWN = -50 };

struct Explorer {
  string path;
  List<File> files;
  sortBy sortedBy = FILE_NAME;
  sortOrder order = ASC;
  Node<File> *activeFile[2] = {nullptr, nullptr};
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

void refreshExplorer(Explorer &explorer, Explorer *activeExplorer, Font &font,
                     ColorTheme theme);

void updateFilesY(List<File> &files, int y);

void updateFilesState(Explorer &explorer, Event event, MouseEventType type,
                      Vector2i &oldClick);

void updateScrollbarState(Explorer &explorer, Event event, MouseEventType type,
                          Vector2i &oldClick);

void updateExplorerIndicator(Explorer *explorer, Explorer *&activeExplorer);

void updateExplorerState(Explorer &explorer, Event event, MouseEventType type,
                         Explorer *&activeExplorer, Vector2i &oldClick,
                         Input *&activeInput, Font &font,
                         ColorTheme theme = {});

void sortFiles(Explorer &explorer, sortBy criteria);

void scrollFiles(Explorer *activeExplorer, Direction d);

void drawFiles(RenderWindow &window, Explorer explorer);

void drawExplorer(RenderWindow &window, Explorer explorer);

void closeExplorer(Explorer &explorer);

#endif
