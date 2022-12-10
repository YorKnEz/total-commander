#ifndef THEME_H
#define THEME_H

#include "element.h"
#include <SFML/Graphics.hpp>

struct ColorTheme {
  Color text, background, border;
  ButtonStateColors buttonStateColors[B_MAX_STATES];
  InputStateColors inputStateColors[I_MAX_STATES];
};

extern ColorTheme dark;

#endif
