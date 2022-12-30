#ifndef MENU_H
#define MENU_H

#include "button.h"
#include "element.h"
#include "explorer.h"
#include "input.h"
#include "textbox.h"

#include "filetree.h"
#include "theme.h"
#include <SFML/Graphics.hpp>
#include <string>

#define POP_UP_DEFAULT_W 400
#define POP_UP_DEFAULT_H 140

using namespace sf;
using namespace std;

enum PopUpType { GET_FILENAME, GET_PATH };
enum MenuButtons {
  RENAME_ENTRY,
  MKDIR,
  COPY_ENTRY,
  DELETE_ENTRY,
  MOVE_ENTRY
};

string createPopUp(int width, int height, PopUpType state, string windowTitle,
                   string textBoxString, string buttonString,
                   string inputPlaceholder, string inputValue, Font &font,
                   int charSize, ColorTheme theme);

void createErrorPopUp(int width, int height, string windowTitle,
                      string errorMessage, Font &font, int charSize,
                      ColorTheme theme);

string checkInput(string inputValue, PopUpType type);

void handleMenuButtons(Explorer *explorer, int explorers,
                       Explorer *activeExplorer, MenuButtons button,
                       string windowTitle, Font &font, int charSize,
                       ColorTheme theme);

#endif
