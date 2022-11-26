#include "utils.h"

// converts a string to an integer
int str2int(string stringNumber) {
  int number = 0;

  for (int i = 0; i < stringNumber.size(); i++) {
    if (stringNumber[i] == ',') {
      continue;
    }

    if ('0' <= stringNumber[i] && stringNumber[i] <= '9') {
      number = number * 10 + int(stringNumber[i] - '0');
    } else {
      return -1;
    }
  }

  return number;
}

// converts an integer to string
string int2str(int a) {
  if (a == -1) {
    return "<DIR>";
  }

  string stringNumber;

  while (a) {
    stringNumber.insert(stringNumber.begin(), char(a % 10 + '0'));
    a /= 10;
  }

  return stringNumber;
}

// parses a string into a Filedata structure
Filedata parseFileDataString(string filedataString) {
  // remove extra spaces from string
  for (int i = 1; i < filedataString.size(); i++) {
    if (filedataString[i] == filedataString[i - 1] &&
        filedataString[i] == ' ') {
      filedataString.erase(i--, 1);
    }
  }

  // the structure of the file data string is:
  // DATE TIME AM/PM <DIR>/SIZE NAME
  Filedata filedata;

  int spacePos = -1;

  for (int i = 0; i < 3; i++) {
    spacePos = filedataString.find(' ', spacePos + 1);
  }

  filedata.date = filedataString.substr(0, spacePos);

  int nextSpace = filedataString.find(' ', spacePos + 1);

  filedata.size =
      str2int(filedataString.substr(spacePos + 1, nextSpace - (spacePos + 1)));

  spacePos = nextSpace;

  filedata.filename = filedataString.substr(
      nextSpace + 1, filedataString.size() - (nextSpace + 1));

  return filedata;
}

// extracts the rgb values from a hexcode and updates an RGB structure
void hexColorToRGB(RGB &color, unsigned int hex) {
  color.r = (hex & 0xFF0000) >> 4 * 4;
  color.g = (hex & 0x00FF00) >> 4 * 2;
  color.b = (hex & 0x0000FF) >> 4 * 0;
}

// extracts the rgb values from a hexcode and returns an RGB structure
RGB getRGBFromHex(unsigned int hex) {
  RGB color;

  color.r = (hex & 0xFF0000) >> 4 * 4;
  color.g = (hex & 0x00FF00) >> 4 * 2;
  color.b = (hex & 0x0000FF) >> 4 * 0;

  return color;
}