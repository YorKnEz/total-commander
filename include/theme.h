#ifndef THEME_H
#define THEME_H

#include "button.h"
#include "dllist.h"
#include "element.h"
#include "file.h"
#include "input.h"
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <stdio.h>
#include <string>

struct ColorTheme {
  Color textHighContrast, textMediumContrast, textLowContrast;
  Color bgBody, bgLowContrast;
  Color border;
  StateColors buttonStateColors[B_MAX_STATES];
  FileStateColors fileStateColors[F_MAX_STATES];
  StateColors inputStateColors[I_MAX_STATES];
};

struct Theme {
  int charSize;
  Font font;
  // icons
  Texture diagram, downArrow, search, upArrow;
  FileIcons fileIcons;
  ColorTheme colors;
};

void printList(List<Theme> l);

void readColor(Color &color);

void readColorTheme(ColorTheme &colors);

// reads the themes file
void loadThemes(List<Theme> &themes);

#endif
