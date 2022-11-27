#ifndef DLLIST_H
#define DLLIST_H
#include <iostream>
#include <string>

using namespace std;

#define MAX_PATH 260
#define MAX_DATE 24

struct Filedata {
  string filename; // windows allows names of max 260 characters
  string ext;
  string size;
  string date; // date is about 20 characters
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

void sort(list &l, bool (*sortCriteria)(node *a, node *b));
#endif