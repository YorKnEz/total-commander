#include "theme.h"

void printList(List<Theme> l) {
  Node<Theme> *p = l.head;
  unsigned int index = 0;

  if (l.length == 0) {
    return;
  }

  do {
    cout << "Char Size: " << p->data.charSize << "\n\n";

    ColorTheme currentColors = p->data.colors;

    cout << "Text High Contrast: " << std::hex
         << currentColors.textHighContrast.toInteger() << "\n";
    cout << "Text Medium Contrast: " << std::hex
         << currentColors.textMediumContrast.toInteger() << "\n";
    cout << "Text Low Contrast: " << std::hex
         << currentColors.textLowContrast.toInteger() << "\n\n";

    cout << "Background Body : " << std::hex << currentColors.bgBody.toInteger()
         << "\n";
    cout << "Background Low Contrast: " << std::hex
         << currentColors.bgLowContrast.toInteger() << "\n\n";

    cout << "Border: " << std::hex << currentColors.border.toInteger()
         << "\n\n";

    for (int i = 0; i < B_MAX_STATES; i++) {
      cout << "Button State " << i << " Colors " << std::hex
           << currentColors.buttonStateColors[i].text.toInteger() << " | ";
      cout << std::hex
           << currentColors.buttonStateColors[i].background.toInteger()
           << " | ";
      cout << std::hex << currentColors.buttonStateColors[i].border.toInteger()
           << "\n";
    }

    cout << "\n";

    for (int i = 0; i < F_MAX_STATES; i++) {
      cout << "Input State " << i << " Colors " << std::hex
           << currentColors.fileStateColors[i].textHighContrast.toInteger()
           << " | ";
      cout << std::hex
           << currentColors.fileStateColors[i].textLowContrast.toInteger()
           << " | ";
      cout << std::hex
           << currentColors.fileStateColors[i].background.toInteger() << " | ";
      cout << std::hex << currentColors.fileStateColors[i].border.toInteger()
           << "\n";
    }

    cout << "\n";

    for (int i = 0; i < I_MAX_STATES; i++) {
      cout << "Input State " << i << " Colors " << std::hex
           << currentColors.inputStateColors[i].text.toInteger() << " | ";
      cout << std::hex
           << currentColors.inputStateColors[i].background.toInteger() << " | ";
      cout << std::hex << currentColors.inputStateColors[i].border.toInteger()
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

void readColorTheme(ColorTheme &colors) {
  readColor(colors.textHighContrast);
  readColor(colors.textMediumContrast);
  readColor(colors.textLowContrast);

  readColor(colors.bgBody);
  readColor(colors.bgLowContrast);

  readColor(colors.border);

  for (int i = 0; i < B_MAX_STATES; i++) {
    readColor(colors.buttonStateColors[i].text);
    readColor(colors.buttonStateColors[i].background);
    readColor(colors.buttonStateColors[i].border);
  }

  for (int i = 0; i < F_MAX_STATES; i++) {
    readColor(colors.fileStateColors[i].textHighContrast);
    readColor(colors.fileStateColors[i].textLowContrast);
    readColor(colors.fileStateColors[i].background);
    readColor(colors.fileStateColors[i].border);
  }

  for (int i = 0; i < I_MAX_STATES; i++) {
    readColor(colors.inputStateColors[i].text);
    readColor(colors.inputStateColors[i].background);
    readColor(colors.inputStateColors[i].border);
  }
}

void loadIcons(Theme &theme, string path) {
  // load theme icons
  theme.diagram.loadFromFile(path + "/diagram.png");
  theme.downArrow.loadFromFile(path + "/down-arrow.png");
  theme.search.loadFromFile(path + "/search.png");
  theme.upArrow.loadFromFile(path + "/up-arrow.png");

  // load file icons
  theme.fileIcons.file.loadFromFile(path + "/file.png");
  theme.fileIcons.folder.loadFromFile(path + "/folder.png");
  theme.fileIcons.image.loadFromFile(path + "/image.png");
  theme.fileIcons.pdf.loadFromFile(path + "/pdf.png");
  theme.fileIcons.txt.loadFromFile(path + "/txt.png");
}

void loadThemes(List<Theme> &themes) {
  themes.init();

  string entry;

  FILE *themesPtr = freopen("assets/themes.txt", "r", stdin);

  if (!themesPtr) {
    return;
  }

  Theme currentTheme;
  ColorTheme currentColors;
  string path;

  // de citit din file de pus in variabila si de adaugat variabila in lista
  while (cin >> entry) {
    cin >> std::dec >> currentTheme.charSize; // read char size
    cin.get();
    getline(cin, path); // get path of the font

    // if loading the font fails, load the default one
    if (!currentTheme.font.loadFromFile(path)) {
      currentTheme.font.loadFromFile("assets/hack.ttf");
    }

    getline(cin, path); // get path of the icons

    // if loading the icons fails, load the default ones
    if (!currentTheme.diagram.loadFromFile(path + "/diagram.png")) {
      loadIcons(currentTheme, "assets/icons/dark");
    } else {
      loadIcons(currentTheme, path);
    }

    readColorTheme(currentTheme.colors); // read the color theme

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

    Theme theme;

    theme.charSize = 12;

    // load font
    theme.font.loadFromFile("assets/hack.ttf");

    loadIcons(theme, "assets/icons/dark");

    // load theme colors
    theme.colors = dark;

    themes.add(theme, 0);
  }

  // make the theme list cyclable
  themes.head->prev = themes.tail;
  themes.tail->next = themes.head;

  fclose(themesPtr);
}
