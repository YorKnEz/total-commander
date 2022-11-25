#include "utils.h"

int str2int(char *stringNumber) {
  int number = 0, len = strlen(stringNumber);

  for (int i = 0; i < len; i++) {
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

char *int2str(int a) {
  if (a < 0)
    return "<DIR>";

  char *s = new char[100];
  int l = 0, ca = a;

  while (ca) {
    l++;
    ca /= 10;
  }

  s[l] = '\0';

  while (a) {
    s[--l] = a % 10 + '0';
    a /= 10;
  }

  return s;
}

Filedata parseFileDataString(char *filedataString) {
  // the structure of the file data string is:
  // DATE TIME AM/PM <DIR>/SIZE NAME
  char *p = strtok(filedataString, " ");

  Filedata filedata;

  strcpy(filedata.date, p);

  p = strtok(NULL, " ");

  for (int i = 0; i < 2; i++) {
    strcat(filedata.date, " ");
    strcat(filedata.date, p);

    p = strtok(NULL, " ");
  }

  // size will be -1 if the file is a directory
  filedata.size = str2int(p);

  p = strtok(NULL, " ");

  strcpy(filedata.filename, p);

  p = strtok(NULL, " ");

  while (p) {
    strcat(filedata.filename, " ");
    strcat(filedata.filename, p);

    p = strtok(NULL, " ");
  }

  return filedata;
}

void hexColorToRGB(RGB &color, unsigned int hex) {
  color.r = (hex & 0xFF0000) >> 4 * 4;
  color.g = (hex & 0x00FF00) >> 4 * 2;
  color.b = (hex & 0x0000FF) >> 4 * 0;
}

RGB getRGBFromHex(unsigned int hex) {
  RGB color;

  color.r = (hex & 0xFF0000) >> 4 * 4;
  color.g = (hex & 0x00FF00) >> 4 * 2;
  color.b = (hex & 0x0000FF) >> 4 * 0;

  return color;
}