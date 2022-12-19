#ifndef MENU_H
#define MENU_H

#include "element.h"
#include "filetree.h"
#include "theme.h"
#include <SFML/Graphics.hpp>

enum PopUpType { GET_FILENAME, GET_PATH };

string createPopUp(int width, int height, PopUpType state, string windowTitle,
                   string textBoxString, string buttonString,
                   string inputPlaceholder, string inputValue, Font &font,
                   int charSize, ColorTheme theme);

void createErrorPopUp(int width, int height, string windowTitle,
                      string errorMessage, Font &font, int charSize,
                      ColorTheme theme);

#endif