#ifndef DLLIST_H
#define DLLIST_H
#include <stdio.h>

#define MAX_PATH 260
#define MAX_DATE 24

struct Filedata {
  char filename[MAX_PATH + 1]; // windows allows names of max 260 characters
  char ext[MAX_PATH + 1];
  int size;
  char date[MAX_DATE + 1]; // date is about 20 characters
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