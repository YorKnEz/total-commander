#ifndef FILE_TREE_H
#define FILE_TREE_H

#include "dllist.h"
#include "element.h"
#include "utils.h"
#include <chrono>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

// define the file separator between linux and windows
#if defined _WIN32
#define SEP "\\"
#elif defined __linux__
#define SEP "/"
#endif

using namespace std;
namespace fs = filesystem;
using namespace chrono_literals;

enum sortBy { FILE_NAME, FILE_EXT, FILE_SIZE, FILE_DATE };

string getDefaultPath();

void getFilesFromPath(list &l, string path, Font &font, int charSize, int x,
                      int y, int width, int height,
                      FileStateColors stateColors[B_MAX_STATES]);

void sortFiletree(list &l, sortBy criteria, sortOrder order);

node *find(list l, string filename);

string evalPath(string path);

bool isValidPath(string path);

string getCurrentFolder(string path);

void openFolder(string &path, string name);

void createFolder(string path, string name);

void copyFile(string fromPath, string toPath);

void copyFolder(string fromPath, string toPath);

void deleteFile(string path);

void deleteFolder(string path);

void moveFile(string fromPath, string toPath);

void moveFolder(string fromPath, string toPath);

void editFileName(string path, string newName);

void editFolderName(string path, string newName);

#endif
