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

using namespace std;
namespace fs = filesystem;
using namespace chrono_literals;

enum sortBy { FILE_NAME, FILE_EXT, FILE_SIZE, FILE_DATE };

// copies a file from a path to another path
void copyFile(string fromPath, string toPath);

// moves a file from a path to another path
void moveFile(string fromPath, string toPath);

// goes into the next folder
void openFolder(string &path, string name);

// renames a file
void editFileName(string path, string newName);

// deletes a file from a specified path
void deleteFile(string path, string fileToDelete);

// deletes a folder (requires confirmation in terminal)
void deleteFolder(string path);

// creates a new folder
void createFolder(string path, string name);

void getFilesFromPath(list &l, string path, Font &font, int charSize, int x,
                      int y, int width, int height,
                      FileStateColors stateColors[B_MAX_STATES]);

void sortFiletree(list &l, sortBy criteria, sortOrder order);

node *find(list l, string filename);

#endif
