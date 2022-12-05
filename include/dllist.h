#ifndef DLLIST_H
#define DLLIST_H
#include <iostream>
#include <string>

using namespace std;

enum sortOrder { DESC = -1, ASC = 1 };

struct Filedata {
  string filename;
  string ext;
  string size;
  string date;
};

struct node {
  Filedata data;
  node *prev;
  node *next;
};

struct list {
  unsigned int length;
  node *head;
  node *tail;
};

void init(list &l);

void add(list &l, Filedata data, unsigned int index);

void remove(list &l, unsigned int index);

void printList(list l);

void free(list l);

void sort(list &l, sortOrder order,
          bool (*sortCriteria)(node *a, node *b, sortOrder order));
#endif
