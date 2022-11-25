#include "element.h"

void drawButton(Button button) {
  if (button.oldState != button.state) {
    switch (button.state) {
    case INACTIVE:
      hexColorToRGB(button.textColor, 0xEB4034);
      hexColorToRGB(button.borderColor, 0xEB4034);
      break;
    case HOVERED:
      hexColorToRGB(button.textColor, 0x7700ff);
      hexColorToRGB(button.borderColor, 0x7700ff);
      break;
    case CLICKED:
      hexColorToRGB(button.textColor, 0x00d5ff);
      hexColorToRGB(button.borderColor, 0x00d5ff);
      break;
    case DCLICKED:
      hexColorToRGB(button.textColor, 0x42f54b);
      hexColorToRGB(button.borderColor, 0x42f54b);
      break;
    }

    setfillstyle(SOLID_FILL,
                 COLOR(button.backgroundColor.r, button.backgroundColor.g,
                       button.backgroundColor.b));
    bar(button.coords.x, button.coords.y, button.coords.x + button.width - 1,
        button.coords.y + button.height - 1);

    setcolor(COLOR(button.borderColor.r, button.borderColor.g,
                   button.borderColor.b));
    setlinestyle(SOLID_LINE, 1, 2);
    rectangle(button.coords.x, button.coords.y,
              button.coords.x + button.width - 1,
              button.coords.y + button.height - 1);

    setcolor(COLOR(button.textColor.r, button.textColor.g, button.textColor.b));
    setbkcolor(COLOR(button.backgroundColor.r, button.backgroundColor.g,
                     button.backgroundColor.b));

    if (textwidth(button.text) > button.width) {
      strcat(button.text, "...");
    }

    int len = strlen(button.text) - 4;

    while (textwidth(button.text) > button.width) {
      strcpy(button.text + len, button.text + len + 1);
    }

    outtextxy(button.coords.x + button.width / 2 - textwidth(button.text) / 2,
              button.coords.y + button.height / 2 - textheight(button.text) / 2,
              button.text);
  }
}

bool isHovered(Button &button, Point mouse) {
  if ((button.coords.x <= mouse.x &&
       mouse.x <= button.coords.x + button.width) &&
      (button.coords.y <= mouse.y &&
       mouse.y <= button.coords.y + button.height)) {
    return true;
  }

  return false;
}

Text createText(int x, int y, int width, int height, char *textString,
                RGB textColor, RGB backgroundColor, RGB borderColor) {
  Text text;

  text.coords.x = x;
  text.coords.y = y;

  text.width = width;
  text.height = height;

  text.text = new char[strlen(textString)];
  strcpy(text.text, textString);

  text.textColor = textColor;
  text.backgroundColor = backgroundColor;
  text.borderColor = borderColor;

  return text;
}

void drawText(Text text) {
  setfillstyle(SOLID_FILL, COLOR(text.backgroundColor.r, text.backgroundColor.g,
                                 text.backgroundColor.b));
  bar(text.coords.x, text.coords.y, text.coords.x + text.width,
      text.coords.y + text.height);

  setcolor(COLOR(text.borderColor.r, text.borderColor.g, text.borderColor.b));
  setlinestyle(SOLID_LINE, 1, THICK_WIDTH);
  rectangle(text.coords.x, text.coords.y, text.coords.x + text.width,
            text.coords.y + text.height);

  setcolor(COLOR(text.textColor.r, text.textColor.g, text.textColor.b));
  setbkcolor(COLOR(text.backgroundColor.r, text.backgroundColor.g,
                   text.backgroundColor.b));

  if (textwidth(text.text) > text.width) {
    strcat(text.text, "...");
  }

  int len = strlen(text.text) - 3;

  while (textwidth(text.text) > text.width) {
    strcpy(text.text + len, text.text + len + 1);
    len--;
  }

  // outtextxy(text.coords.x + text.width / 2 - textwidth(text.text) / 2,
  //           text.coords.y + text.height / 2 - textheight(text.text) / 2,
  //           text.text);
  outtextxy(text.coords.x, text.coords.y, text.text);
}

void drawFile(File file) {
  Text filename, size, date;

  filename = createText(file.coords.x, file.coords.y, file.filenameColumn,
                        file.height, file.data.filename, file.textColor,
                        file.backgroundColor, file.borderColor);

  char sizeString[100];
  strcpy(sizeString, int2str(file.data.size));

  size = createText(file.coords.x + filename.width, file.coords.y,
                    file.sizeColumn, file.height, sizeString, file.textColor,
                    file.backgroundColor, file.borderColor);

  date = createText(file.coords.x + filename.width + size.width, file.coords.y,
                    file.dateColumn, file.height, file.data.date,
                    file.textColor, file.backgroundColor, file.borderColor);

  drawText(filename);
  drawText(size);
  drawText(date);
}

bool isHovered(File &file, Point mouse) {
  if ((file.coords.x <= mouse.x && mouse.x <= file.coords.x + file.width) &&
      (file.coords.y <= mouse.y && mouse.y <= file.coords.y + file.height)) {
    return true;
  }

  return false;
}