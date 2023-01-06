#ifndef DLLIST_FILE_H
#define DLLIST_FILE_H

#include "dllist.h"
#include "file.h"

void sort(List<File> &l, sortOrder order,
          bool (*sortCriteria)(Node<File> *a, Node<File> *b, sortOrder order));

void printList(List<File> l);

// graphics related functions

// updates the y screen position of each file
void updateFilesY(List<File> &files, int y);

// checks if any of the files has been clicked and stores it in
// `activeFile`
void updateFilesState(List<File> &files, Node<File> *activeFile[2], Event event,
                      MouseEventType type, Vector2i &oldClick);

void updateFilesTheme(List<File> &list,
                      FileStateColors fileStateColors[F_MAX_STATES]);

// draws the files on the screen
void drawFiles(RenderWindow &window, List<File> list, int miny, int maxy);

#endif