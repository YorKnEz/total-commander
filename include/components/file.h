#ifndef FILE_H
#define FILE_H

#include "element.h"

#define F_MAX_STATES 3

enum FileState { F_INACTIVE, F_SELECTED, F_DCLICKED };

struct Filedata {
  string filename;
  string ext;
  string size;
  string date;
};

struct FileStateColors {
  Color textHighContrast, textLowContrast, background, border;
};

struct File {
  FileStateColors stateColors[F_MAX_STATES];
  RectangleShape background;
  Filedata data;
  int filenameColumn, extColumn, sizeColumn, dateColumn;
  Text filename, ext, size, date;
  FileState state;
};

File createFile(Filedata data, Font &font, int charSize, int x, int y,
                int width, int height,
                FileStateColors stateColors[F_MAX_STATES],
                int borderThickness = 0);

void updateFileTheme(File &file, FileStateColors stateColors[F_MAX_STATES]);

void drawFile(RenderWindow &window, File file, bool filenameOnly = false);

#endif
