#ifndef THEME_H
#define THEME_H

#include "element.h"
#include <SFML/Graphics.hpp>

struct ColorTheme {
  Color textHighContrast, textMediumContrast, textLowContrast;
  Color bgBody, bgLowContrast;
  Color border;
  StateColors buttonStateColors[B_MAX_STATES];
  FileStateColors fileStateColors[B_MAX_STATES];
  StateColors inputStateColors[I_MAX_STATES];
};

extern ColorTheme dark;

#endif
