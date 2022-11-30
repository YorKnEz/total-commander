#include "element.h"

Button createButton(string text, Font &font, int x, int y, int width,
                    int height, ButtonStateColors buttonStateColors[MAX_STATES],
                    unsigned int borderThickness) {
  Button button;
  button.state = INACTIVE; // set the state of the button
  button.fullText =
      text; // save the string that is supposed to be shown on the screen

  // copy the state colors
  for (int i = 0; i < MAX_STATES; i++) {
    button.buttonStateColors[i] = buttonStateColors[i];
  }

  // initialize the background of the button
  button.background.setSize(Vector2f(width, height));
  button.background.setFillColor(buttonStateColors[INACTIVE].background);
  button.background.setOutlineColor(buttonStateColors[INACTIVE].primary);
  button.background.setOutlineThickness(borderThickness);
  button.background.setPosition(x, y);

  // initialize the text of the button
  button.text = createText(text, font, 30, x, y,
                           button.background.getGlobalBounds().width -
                               2 * (borderThickness + 1),
                           buttonStateColors[INACTIVE].primary);

  // set the offset to the text relative to the button
  int offsetX = button.background.getGlobalBounds().left +
                button.background.getGlobalBounds().width / 2 -
                button.text.getGlobalBounds().left -
                button.text.getGlobalBounds().width / 2;

  int offsetY = button.background.getGlobalBounds().top +
                button.background.getGlobalBounds().height / 2 -
                button.text.getGlobalBounds().top -
                button.text.getGlobalBounds().height / 2;

  button.text.setPosition(x + offsetX, y + offsetY);

  return button;
}

void updateButtonState(Button &button, Event event, MouseEventType type) {
  button.oldState = button.state;

  switch (type) {
  case RELEASE:
    if (button.state == CLICKED || button.state == DCLICKED) {
      button.state = isHovered(button, event.mouseButton.x, event.mouseButton.y)
                         ? HOVERED
                         : INACTIVE;
    }
    break;
  case DCLICK:
    button.state = isHovered(button, event.mouseButton.x, event.mouseButton.y)
                       ? DCLICKED
                       : INACTIVE;
    break;
  case CLICK:
    button.state = isHovered(button, event.mouseButton.x, event.mouseButton.y)
                       ? CLICKED
                       : INACTIVE;
    break;
  case MOVE:
    if (button.state != CLICKED && button.state != DCLICKED) {
      button.state = isHovered(button, event.mouseMove.x, event.mouseMove.y)
                         ? HOVERED
                         : INACTIVE;
    }
    break;
  }
}

void drawButton(RenderWindow &window, Button button) {
  // if (button.oldState != button.state) {
  Color textColor, borderColor;

  // update the color of the button depending on it's state
  button.text.setFillColor(button.buttonStateColors[button.state].primary);
  button.background.setOutlineColor(
      button.buttonStateColors[button.state].primary);

  // draw the button on the window passed by reference
  window.draw(button.background);
  window.draw(button.text);
  // }
}

bool isHovered(Button &button, int mouseX, int mouseY) {
  Vector2f buttonPosition = button.background.getPosition();
  FloatRect buttonBounds = button.background.getGlobalBounds();

  if ((buttonBounds.left <= mouseX &&
       mouseX <= buttonBounds.left + buttonBounds.width) &&
      (buttonBounds.top <= mouseY &&
       mouseY <= buttonBounds.top + buttonBounds.height)) {
    return true;
  }

  return false;
}

Text createText(string textString, Font &font, int charSize, int x, int y,
                int width, Color textColor) {
  Text text;
  text.setFont(font);
  text.setPosition(x, y);
  text.setCharacterSize(charSize);
  text.setStyle(Text::Regular);
  text.setString(textString);
  text.setFillColor(textColor);
  // shrink the text that is shown on the screen to avoid overflow
  if (text.getGlobalBounds().width > width) {
    textString.append("..");
    text.setString(textString);
  }

  while (text.getGlobalBounds().width > width) {
    textString.erase(textString.size() - 3, 1);
    text.setString(textString);
  }
  return text;
}

void drawText(RenderWindow &window, Text text) {
  window.draw(text);
  //   setfillstyle(SOLID_FILL, COLOR(text.backgroundColor.r,
  //   text.backgroundColor.g,
  //                                  text.backgroundColor.b));
  //   bar(text.coords.x, text.coords.y, text.coords.x + text.width,
  //       text.coords.y + text.height);

  //   setcolor(COLOR(text.borderColor.r, text.borderColor.g,
  //   text.borderColor.b)); setlinestyle(SOLID_LINE, 1, THICK_WIDTH);
  //   rectangle(text.coords.x, text.coords.y, text.coords.x + text.width,
  //             text.coords.y + text.height);

  //   setcolor(COLOR(text.textColor.r, text.textColor.g, text.textColor.b));
  //   setbkcolor(COLOR(text.backgroundColor.r, text.backgroundColor.g,
  //                    text.backgroundColor.b));

  //   char *textString = new char[text.text.size() + 3];
  //   strcpy(textString, text.text.c_str());

  //   if (textwidth(textString) > text.width) {
  //     strcat(textString, "..");
}

//   int len = text.text.size() - 1;

//   while (textwidth(textString) > text.width) {
//     strcpy(textString + len, textString + len + 1);
//     len--;
//   }

//   // outtextxy(button.coords.x + button.width / 2 - textwidth(textString) /
//   2,
//   //           button.coords.y + button.height / 2 - textheight(textString) /
//   //           2, textString);

//   outtextxy(text.coords.x, text.coords.y, textString);
// }

// void drawFile(File file) {
//   Text filename, ext, size, date;

//   filename = createText(file.coords.x, file.coords.y, file.filenameColumn,
//                         file.height, file.data.filename, file.textColor,
//                         file.backgroundColor, file.borderColor);

//   ext = createText(file.coords.x + filename.width, file.coords.y,
//                    file.extColumn, file.height, file.data.ext,
//                    file.textColor, file.backgroundColor, file.borderColor);

//   size = createText(file.coords.x + filename.width + ext.width,
//   file.coords.y,
//                     file.sizeColumn, file.height, file.data.size,
//                     file.textColor, file.backgroundColor, file.borderColor);

//   date = createText(file.coords.x + filename.width + ext.width + size.width,
//                     file.coords.y, file.dateColumn, file.height,
//                     file.data.date, file.textColor, file.backgroundColor,
//                     file.borderColor);

//   drawText(filename);
//   drawText(ext);
//   drawText(size);
//   drawText(date);
// }

// bool isHovered(File &file, Point mouse) {
//   if ((file.coords.x <= mouse.x && mouse.x <= file.coords.x + file.width) &&
//       (file.coords.y <= mouse.y && mouse.y <= file.coords.y + file.height)) {
//     return true;
//   }

//   return false;
// }