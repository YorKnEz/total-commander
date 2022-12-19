#ifndef ELEMENT_H
#define ELEMENT_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

using namespace sf;
using namespace std;

#define B_MAX_STATES 4
#define I_MAX_STATES 3
#define F_MAX_STATES 3

enum MouseEventType { MOVE, RELEASE, CLICK, DCLICK };
enum ButtonState { B_INACTIVE, B_HOVERED, B_CLICKED, B_DCLICKED };
enum InputState { I_INACTIVE, I_HOVERED, I_ACTIVE };
enum FileState { F_INACTIVE, F_SELECTED, F_DCLICKED };

struct StateColors {
  Color text, background, border;
};

struct TextBox {
  RectangleShape background;
  string fullText;
  Text text;
};

struct Button {
  StateColors stateColors[B_MAX_STATES];
  RectangleShape background;
  string fullText;
  Text text;
  ButtonState state;
};

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

struct Input {
  StateColors stateColors[I_MAX_STATES];
  RectangleShape background;
  string placeholder, value;
  Text displayText;
  InputState state;
  int cursorLocation;
  int startPosition; // used if the displayText is larger than the box
  int displayLength; // max length that can be displayed at once
};

struct Scrollbar {
  Button up, down;
  int scrollableHeight, scrollOffset;
  RectangleShape track, thumb;
  ButtonState state;
};

// helpful functions
bool isHovered(FloatRect box, int mouseX, int mouseY);

RectangleShape createRect(int x, int y, int width, int height, Color fill,
                          Color outline = Color::Transparent,
                          int borderThickness = 0);

// text functions
Text createText(string textString, Font &font, int charSize, int x, int y,
                int width, Color text);

void updateText(Text &text, string newText, FloatRect bounds);

void drawText(RenderWindow &window, Text text);

// textbox functions
TextBox createTextBox(string textString, Font &font, int charSize, int x, int y,
                      int width, int height, Color text = Color::Transparent,
                      Color bg = Color::Transparent,
                      Color border = Color::Transparent,
                      int borderThickness = 0);

void drawTextBox(RenderWindow &window, TextBox textbox);

// button functions
Button createButton(string text, Font &font, int charSize, int x, int y,
                    int width, int height,
                    StateColors stateColors[B_MAX_STATES],
                    unsigned int borderThickness = 0);

void drawButton(RenderWindow &window, Button button);

void updateButtonState(Button &button, Event event, MouseEventType type,
                       Vector2i &oldClick);

// file functions
File createFile(Filedata data, Font &font, int charSize, int x, int y,
                int width, int height,
                FileStateColors stateColors[F_MAX_STATES],
                int borderThickness = 0);

void updateFileState(File &file, Event event, MouseEventType type,
                     File *activeFile[2]);

void drawFile(RenderWindow &window, File file);

// input functions
Input createInput(string placeholder, string value, Font &font, int charSize,
                  int x, int y, int width, int height,
                  StateColors stateColors[I_MAX_STATES],
                  unsigned int borderThickness = 0);

void updateInputState(Input &input, Event event, MouseEventType type,
                      Input *&activeInput);

void shrinkInput(Input &input);

void drawInput(RenderWindow &window, Input &input);

void drawCursor(RenderWindow &window, Input *activeInput);

Scrollbar createScrollbar(Font &font, int charSize, int x, int y, int width,
                          int height, int scrollableHeight, int btnHeight,
                          StateColors stateColors[B_MAX_STATES],
                          unsigned int borderThickness = 0);

void updateScrollableHeight(Scrollbar &scrollbar, int scrollableHeight);

void updateScrollbar(Scrollbar &scrollbar, int scrollOffset);

void updateScrollbarOnDrag(Scrollbar &scrollbar, int &scrollOffset);

void drawScrollbar(RenderWindow &window, Scrollbar scrollbar);

#endif
