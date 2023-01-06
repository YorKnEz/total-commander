#ifndef FILE_TREE_H
#define FILE_TREE_H

#include "dllist-file.h"
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

void getFilesFromPath(List<File> &l, string path, Font &font, int charSize,
                      int x, int y, int width, int height,
                      FileStateColors stateColors[F_MAX_STATES], bool ignoreBackwardsFolder = false);

void searchFile(List<File> &search, string path, string nameToSearch);

void sortFiletree(List<File> &l, sortBy criteria, sortOrder order);

Node<File> *find(List<File> l, string filename);

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
