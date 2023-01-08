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

void readColor(ifstream &theme, Color &color) {
  unsigned int colorCode;
  theme >> std::hex >> colorCode;
  color = Color(colorCode);
}

void readColorTheme(ifstream &theme, ColorTheme &colors) {
  readColor(theme, colors.textHighContrast);
  readColor(theme, colors.textMediumContrast);
  readColor(theme, colors.textLowContrast);

  readColor(theme, colors.bgBody);
  readColor(theme, colors.bgLowContrast);

  readColor(theme, colors.border);

  for (int i = 0; i < B_MAX_STATES; i++) {
    readColor(theme, colors.buttonStateColors[i].text);
    readColor(theme, colors.buttonStateColors[i].background);
    readColor(theme, colors.buttonStateColors[i].border);
  }

  for (int i = 0; i < F_MAX_STATES; i++) {
    readColor(theme, colors.fileStateColors[i].textHighContrast);
    readColor(theme, colors.fileStateColors[i].textLowContrast);
    readColor(theme, colors.fileStateColors[i].background);
    readColor(theme, colors.fileStateColors[i].border);
  }

  for (int i = 0; i < I_MAX_STATES; i++) {
    readColor(theme, colors.inputStateColors[i].text);
    readColor(theme, colors.inputStateColors[i].background);
    readColor(theme, colors.inputStateColors[i].border);
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

  for (const auto &entry : fs::directory_iterator("assets/themes/")) {
    string entryPath = entry.path();

    // check if the current file is a theme
    if (entryPath.substr(entryPath.size() - 6) == ".theme" &&
        entryPath.find("template.theme") == string::npos) {
      ifstream themeFile;
      themeFile.open(entryPath);

      Theme currentTheme;
      ColorTheme currentColors;

      currentTheme.name = entryPath.substr(entryPath.find_last_of("/") + 1,
                                           entryPath.find_last_of(".") -
                                               entryPath.find_last_of("/") - 1);

      // de citit din file de pus in variabila si de adaugat variabila in lista
      themeFile >> std::dec >> currentTheme.charSize; // read char size
      themeFile.get();

      string path;

      getline(themeFile, path); // get path of the font

      // if loading the font fails, load the default one
      if (!currentTheme.font.loadFromFile(path)) {
        currentTheme.font.loadFromFile("assets/fonts/hack.ttf");
      }

      getline(themeFile, path); // get path of the icons

      // if loading the icons fails, load the default ones
      if (!currentTheme.diagram.loadFromFile(path + "/diagram.png")) {
        loadIcons(currentTheme, "assets/icons/dark");
      } else {
        loadIcons(currentTheme, path);
      }

      readColorTheme(themeFile, currentTheme.colors); // read the color theme

      themes.add(currentTheme, themes.length);

      themeFile.close();
    }
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

    theme.name = "dark";
    theme.charSize = 12;

    // load font
    theme.font.loadFromFile("assets/fonts/hack.ttf");

    loadIcons(theme, "assets/icons/dark");

    // load theme colors
    theme.colors = dark;

    themes.add(theme, 0);
  }

  // make the theme list cyclable
  themes.head->prev = themes.tail;
  themes.tail->next = themes.head;
}
