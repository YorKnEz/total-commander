#ifndef THEME_H
#define THEME_H

#include "button.h"
#include "dllist.h"
#include "element.h"
#include "file.h"
#include "input.h"
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
namespace fs = filesystem;

struct ColorTheme {
  Color textHighContrast, textMediumContrast, textLowContrast;
  Color bgBody, bgLowContrast;
  Color border;
  StateColors buttonStateColors[B_MAX_STATES];
  FileStateColors fileStateColors[F_MAX_STATES];
  StateColors inputStateColors[I_MAX_STATES];
};

struct Theme {
  string name;
  int charSize;
  Font font;
  // icons
  Texture diagram, downArrow, search, upArrow;
  FileIcons fileIcons;
  ColorTheme colors;
};

void printList(List<Theme> l);

void readColor(ifstream &theme, Color &color);

void readColorTheme(ifstream &theme, ColorTheme &colors);

// reads the themes file
void loadThemes(List<Theme> &themes);

#endif
