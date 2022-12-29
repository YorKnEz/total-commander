#ifndef MENU_H
#define MENU_H

#include "element.h"
#include "button.h"
#include "input.h"
#include "textbox.h"

#include "filetree.h"
#include "theme.h"
#include <string>
#include <SFML/Graphics.hpp>

#define POP_UP_DEFAULT_W 400
#define POP_UP_DEFAULT_H 140

using namespace sf;
using namespace std;

enum PopUpType { GET_FILENAME, GET_PATH };

string createPopUp(int width, int height, PopUpType state, string windowTitle,
                   string textBoxString, string buttonString,
                   string inputPlaceholder, string inputValue, Font &font,
                   int charSize, ColorTheme theme);

void createErrorPopUp(int width, int height, string windowTitle,
                      string errorMessage, Font &font, int charSize,
                      ColorTheme theme);

#endif
