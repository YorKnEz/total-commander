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

void printList(List<ColorTheme> l);

void readColor(Color &color);

// reads the themes file
void loadThemes(List<ColorTheme> &themes);

#endif
