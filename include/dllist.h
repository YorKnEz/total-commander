#ifndef DLLIST_H
#define DLLIST_H

#include "file.h"
#include "theme.h"
#include <iostream>
#include <string>

using namespace std;

enum sortOrder { DESC = -1, ASC = 1 };

template <typename T> struct Node {
  T data;
  Node<T> *prev;
  Node<T> *next;
};

template <typename T> struct List {
  unsigned int length;
  Node<T> *head;
  Node<T> *tail;
};

template <typename T> void init(List<T> &l);

template <typename T> void add(List<T> &l, T data, unsigned int index);

template <typename T> void remove(List<T> &l, unsigned int index);

void printList(List<File> l);
void printList(List<ColorTheme> l);

template <typename T> void free(List<T> &l);

void sort(List<File> &l, sortOrder order,
          bool (*sortCriteria)(Node<File> *a, Node<File> *b, sortOrder order));

// graphics related functions

// updates the y screen position of each file
void updateFilesY(List<File> &files, int y);

// checks if any of the files has been clicked and stores it in
// `activeFile`
void updateFilesState(List<File> &files, Node<File> *activeFile[2], Event event,
                      MouseEventType type, Vector2i &oldClick);

// draws the files on the screen
void drawFiles(RenderWindow &window, List<File> list, int miny, int maxy);

#endif
