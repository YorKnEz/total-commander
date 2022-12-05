#ifndef THEME_H
#define THEME_H
#include "element.h"
#include "utils.h"

struct ColorTheme {
  Color text, background, border;
  ButtonStateColors buttonStateColors[B_MAX_STATES];
};

ColorTheme dark = {Color(0xEEEEEEFF),
                   Color(0x242424FF),
                   Color(0xFFFFFFFF),
                   {{Color(0xEB4034FF), Color(0xEEEEEEFF)},
                    {Color(0x7700ffFF), Color(0xEEEEEEFF)},
                    {Color(0x00d5ffFF), Color(0xEEEEEEFF)},
                    {Color(0x42f54bFF), Color(0xEEEEEEFF)}}};
#endif