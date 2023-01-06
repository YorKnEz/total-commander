#include "theme.h"

void printList(List<ColorTheme> l) {
  Node<ColorTheme> *p = l.head;
  unsigned int index = 0;

  if (l.length == 0) {
    return;
  }

  do {
    cout << "Text High Contrast: " << std::hex
         << p->data.textHighContrast.toInteger() << "\n";
    cout << "Text Medium Contrast: " << std::hex
         << p->data.textMediumContrast.toInteger() << "\n";
    cout << "Text Low Contrast: " << std::hex
         << p->data.textLowContrast.toInteger() << "\n\n";

    cout << "Background Body : " << std::hex << p->data.bgBody.toInteger()
         << "\n";
    cout << "Background Low Contrast: " << std::hex
         << p->data.bgLowContrast.toInteger() << "\n\n";

    cout << "Border: " << std::hex << p->data.border.toInteger() << "\n\n";

    for (int i = 0; i < B_MAX_STATES; i++) {
      cout << "Button State " << i << " Colors " << std::hex
           << p->data.buttonStateColors[i].text.toInteger() << " | ";
      cout << std::hex << p->data.buttonStateColors[i].background.toInteger()
           << " | ";
      cout << std::hex << p->data.buttonStateColors[i].border.toInteger()
           << "\n";
    }

    cout << "\n";

    for (int i = 0; i < F_MAX_STATES; i++) {
      cout << "Input State " << i << " Colors " << std::hex
           << p->data.fileStateColors[i].textHighContrast.toInteger() << " | ";
      cout << std::hex << p->data.fileStateColors[i].textLowContrast.toInteger()
           << " | ";
      cout << std::hex << p->data.fileStateColors[i].background.toInteger()
           << " | ";
      cout << std::hex << p->data.fileStateColors[i].border.toInteger() << "\n";
    }

    cout << "\n";

    for (int i = 0; i < I_MAX_STATES; i++) {
      cout << "Input State " << i << " Colors " << std::hex
           << p->data.inputStateColors[i].text.toInteger() << " | ";
      cout << std::hex << p->data.inputStateColors[i].background.toInteger()
           << " | ";
      cout << std::hex << p->data.inputStateColors[i].border.toInteger()
           << "\n";
    }

    cout << "\n";

    p = p->next;
  } while (p != l.head);
}

void readColor(Color &color) {
  unsigned int colorCode;
  cin >> std::hex >> colorCode;
  color = Color(colorCode);
}

void loadThemes(List<ColorTheme> &themes) {
  themes.init();

  string entry;

  FILE *themesPtr = freopen("assets/themes.txt", "r", stdin);

  if (themesPtr == NULL) {
    return;
  }

  ColorTheme currentTheme;
  // de citit din file de pus in variabila si de adaugat variabila in lista
  while (cin >> entry) {
    readColor(currentTheme.textHighContrast);
    readColor(currentTheme.textMediumContrast);
    readColor(currentTheme.textLowContrast);

    readColor(currentTheme.bgBody);
    readColor(currentTheme.bgLowContrast);

    readColor(currentTheme.border);

    for (int i = 0; i < B_MAX_STATES; i++) {
      readColor(currentTheme.buttonStateColors[i].text);
      readColor(currentTheme.buttonStateColors[i].background);
      readColor(currentTheme.buttonStateColors[i].border);
    }

    for (int i = 0; i < F_MAX_STATES; i++) {
      readColor(currentTheme.fileStateColors[i].textHighContrast);
      readColor(currentTheme.fileStateColors[i].textLowContrast);
      readColor(currentTheme.fileStateColors[i].background);
      readColor(currentTheme.fileStateColors[i].border);
    }

    for (int i = 0; i < I_MAX_STATES; i++) {
      readColor(currentTheme.inputStateColors[i].text);
      readColor(currentTheme.inputStateColors[i].background);
      readColor(currentTheme.inputStateColors[i].border);
    }

    themes.add(currentTheme, themes.length);
  }

  // add the default theme if the file is empty
  if (themes.length == 0) {
    ColorTheme dark = {
        Color(0xFFFFFFFF), // text with high contrast
        Color(0x999995FF), // text with medium contrast
        Color(0x5B5C55FF), // text with low contrast
        Color(0x32332BFF), // background of body
        Color(0x191A16FF), // background with low contrast
        Color(0x0A0A09FF), // border
        {{Color(0x848580FF), Color(0x282922FF), Color(0x0A0A09FF)},
         {Color(0xC2C2BFFF), Color(0x282922FF), Color(0x191A16FF)},
         {Color(0xFFFFFFFF), Color(0x282922FF), Color(0x0A0A09FF)},
         {Color(0xFFFFFFFF), Color(0x282922FF), Color(0x0A0A09FF)}},
        {{Color(0x999995FF), Color(0x5B5C55FF), Color(0x32332BFF),
          Color(0x0A0A09FF)},
         {Color(0x999995FF), Color(0x5B5C55FF), Color(0x00FF0032),
          Color(0x0A0A09FF)},
         {Color(0x999995FF), Color(0x5B5C55FF), Color(0x0000FF32),
          Color(0x0A0A09FF)}},
        {{Color(0x848580FF), Color(0x191A16FF), Color(0x0A0A09FF)},
         {Color(0xC2C2BFFF), Color(0x191A16FF), Color(0x191A16FF)},
         {Color(0xFFFFFFFF), Color(0x191A16FF), Color(0x0A0A09FF)}}};

    themes.add(dark, 0);
  }

  themes.head->prev = themes.tail;
  themes.tail->next = themes.head;

  fclose(themesPtr);
}
