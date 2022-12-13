#ifndef THEME_H
#define THEME_H

#include "element.h"
#include <SFML/Graphics.hpp>

struct ColorTheme {
  StateColors buttonStateColors[B_MAX_STATES];
  StateColors inputStateColors[I_MAX_STATES];
};

extern ColorTheme dark;

#endif
