#include "dllist.h"

// initializez an empty double linked list
void init(list &l) {
  l.length = 0;
  l.head = l.tail = NULL;
}

// adds an element of type Filedata at index
void add(list &l, Filedata data, unsigned int index) {
  if (index > l.length)
    return;

  node *n = new node;
  n->next = n->prev = NULL;
  n->data = data;

  if (l.length == 0) {
    l.head = l.tail = n;
  } else if (index == 0) {
    n->next = l.head;
    l.head->prev = n;
    l.head = n;
  } else if (index == l.length) {
    n->prev = l.tail;
    l.tail->next = n;
    l.tail = n;
  } else {
    node *p = l.head;
    unsigned int cIndex = 0;

    while (cIndex < index) {
      p = p->next;
      cIndex++;
    }

    n->next = p;
    n->prev = p->prev;

    p->prev->next = n;
    p->prev = n;
  }

  l.length++;
}

// remove an element at index
void remove(list &l, unsigned int index) {
  if (index >= l.length)
    return;

  if (l.length == 1) {
    l.head = l.tail = NULL;
  } else if (index == 0) {
    l.head = l.head->next;
    delete l.head->prev;
    l.head->prev = 0;
  } else if (index == l.length - 1) {
    l.tail = l.tail->prev;
    delete l.tail->next;
    l.tail->next = 0;
  } else {
    node *p = l.head;
    unsigned int cIndex = 0;

    while (cIndex < index) {
      p = p->next;
      cIndex++;
    }

    p->prev->next = p->next;
    p->next->prev = p->prev;

    delete p;
  }

  l.length--;
}

// shows in the console the contents of a list
void printList(list l) {
  node *p = l.head;
  unsigned int index = 0;

  while (p != NULL) {
    printf("Name: %s | Size: %d | Date: %s\n", p->data.filename, p->data.size,
           p->data.date);

    p = p->next;
  }

  printf("\n");
}

void free(list l) {
  if (l.head) {
    node *p = l.head->next;

    while (p) {
      delete p->prev;

      p = p->next;
    }

    delete p;
  }

  // printf("\nFreed list\n");
}

void sort(list &l, bool (*sortCriteria)(node *a, node *b)) {
  node *p = l.head, *q;
  Filedata aux;

  while (p->next) {
    q = p->next;

    while (q) {
      if (sortCriteria(q, p)) {
        aux = p->data;
        p->data = q->data;
        q->data = aux;
      }

      q = q->next;
    }

    p = p->next;
  }
}
