#include "utils.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct Settings {
  string defaultTheme; 
  int explorers;
};

void loadSettings(Settings &settings);
