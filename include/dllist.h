#ifndef DLLIST_H
#define DLLIST_H

#include "element.h"
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

void free(list l);

void sort(list &l, sortOrder order,
          bool (*sortCriteria)(node *a, node *b, sortOrder order));

#endif
