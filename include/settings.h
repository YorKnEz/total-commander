#include "utils.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct Settings {
  string defaultTheme = "dark"; 
  int explorers = 2;
};

void loadSettings(Settings &settings);
