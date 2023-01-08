#include "settings.h"

void loadSettings(Settings &settings) {
  ifstream settingsFile;
  settingsFile.open("assets/settings.txt");

  string setting;

  while (getline(settingsFile, setting)) {
    string settingKey = setting.substr(0, setting.find(" "));
    string settingValue = setting.substr(setting.find(" ") + 1);

    if (settingKey == "default-theme:") {
      settings.defaultTheme = settingValue;
    } else if (settingKey == "explorers:") {
      settings.explorers = str2int(settingValue);
    }
  }

  settingsFile.close();
}
