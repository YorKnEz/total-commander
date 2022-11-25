#ifndef THEME_H
#define THEME_H
#include "utils.h"

struct ColorTheme {
  RGB text, background, border;
};

ColorTheme dark = {getRGBFromHex(0xEEEEEE), getRGBFromHex(0x242424),
                   getRGBFromHex(0xFFFFFF)};
#endif