#ifndef FILE_TREE_H
#define FILE_TREE_H

#include "dllist-file.h"
#include "theme.h"
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
#define ILLEGAL_SYMBOLS "\\/:*?\"<>|\n"
#elif defined __linux__
#define SEP "/"
#define ILLEGAL_SYMBOLS "/\n"
#endif

using namespace std;
namespace fs = filesystem;
using namespace chrono_literals;

enum sortBy { FILE_NAME, FILE_EXT, FILE_SIZE, FILE_DATE };

string getDefaultPath();

File getFileFromEntry(const auto entry, Theme &theme, int x, int y, int width,
                      int height);

void getFilesFromPath(List<File> &l, string path, int x, int y, int width,
                      int height, Theme &theme,
                      bool ignoreBackwardsFolder = false);

void searchFile(List<File> &results, string path, string searchQuery,
                Theme &theme, int x, int y, int width, int height);

void sortFiletree(List<File> &l, sortBy criteria, sortOrder order);

string evalPath(string path);

bool isValidFilename(string path);

bool isValidPath(string path);

string getSizeOfDrive(string path);

string getCurrentFolder(string path);

void openEntry(string &path, string name, string ext);

void openFile(string path, string name, string ext);

void openFolder(string &path, string name);

void createFolder(string path, string name);

void copyEntry(string fromPath, string toPath);

void copyFile(string fromPath, string toPath);

void copyFolder(string fromPath, string toPath);

void deleteEntry(string path);

void deleteFile(string path);

void deleteFolder(string path);

void moveEntry(string fromPath, string toPath);

void moveFile(string fromPath, string toPath);

void moveFolder(string fromPath, string toPath);

void editEntryName(string path, string newName);

void editFileName(string path, string newName);

void editFolderName(string path, string newName);

#endif
