#ifndef DLLIST_H
#define DLLIST_H

#include "file.h"
#include <iostream>
#include <string>

using namespace std;

enum sortOrder { DESC = -1, ASC = 1 };

struct node {
  File data;
  node *prev;
  node *next;
};

struct list {
  unsigned int length;
  node *head;
  node *tail;
};

void init(list &l);

void add(list &l, File data, unsigned int index);

void remove(list &l, unsigned int index);

void printList(list l);

void free(list &l);

void sort(list &l, sortOrder order,
          bool (*sortCriteria)(node *a, node *b, sortOrder order));

// graphics related functions

// updates the y screen position of each file
void updateFilesY(list &files, int y);

// checks if any of the files has been clicked and stores it in
// `activeFile`
void updateFilesState(list &files, node *activeFile[2], Event event,
                      MouseEventType type, Vector2i &oldClick);

// draws the files on the screen
void drawFiles(RenderWindow &window, list list, int miny, int maxy);

#endif
