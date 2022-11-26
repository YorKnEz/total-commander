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

    char *textString = new char[button.text.size() + 3];
    strcpy(textString, button.text.c_str());

    if (textwidth(textString) > button.width) {
      strcat(textString, "..");
    }

    int len = button.text.size() - 1;

    while (textwidth(textString) > button.width) {
      strcpy(textString + len, textString + len + 1);
      len--;
    }

    outtextxy(button.coords.x + button.width / 2 - textwidth(textString) / 2,
              button.coords.y + button.height / 2 - textheight(textString) / 2,
              textString);
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

Text createText(int x, int y, int width, int height, string textString,
                RGB textColor, RGB backgroundColor, RGB borderColor) {
  Text text;

  text.coords.x = x;
  text.coords.y = y;

  text.width = width;
  text.height = height;

  text.text = textString;

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

  char *textString = new char[text.text.size() + 3];
  strcpy(textString, text.text.c_str());

  if (textwidth(textString) > text.width) {
    strcat(textString, "..");
  }

  int len = text.text.size() - 1;

  while (textwidth(textString) > text.width) {
    strcpy(textString + len, textString + len + 1);
    len--;
  }

  // outtextxy(button.coords.x + button.width / 2 - textwidth(textString) / 2,
  //           button.coords.y + button.height / 2 - textheight(textString) / 2,
  //           textString);

  outtextxy(text.coords.x, text.coords.y, textString);
}

void drawFile(File file) {
  Text filename, size, date;

  filename = createText(file.coords.x, file.coords.y, file.filenameColumn,
                        file.height, file.data.filename, file.textColor,
                        file.backgroundColor, file.borderColor);

  string sizeString = int2str(file.data.size);

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