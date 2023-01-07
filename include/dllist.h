#ifndef DLLIST_H
#define DLLIST_H

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

  // initializez an empty double linked list
  void init() {
    length = 0;
    head = tail = nullptr;
  }

  // adds an element of type File at index
  void add(T data, unsigned int index) {
    if (index > length)
      return;

    Node<T> *n = new Node<T>;
    n->next = n->prev = nullptr;
    n->data = data;

    if (length == 0) {
      head = tail = n;
    } else if (index == 0) {
      n->next = head;
      head->prev = n;
      head = n;
    } else if (index == length) {
      n->prev = tail;
      tail->next = n;
      tail = n;
    } else {
      Node<T> *p = head;
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

    length++;
  }

  // remove an element at index
  void remove(unsigned int index) {
    if (index >= length)
      return;

    if (length == 1) {
      head = tail = nullptr;
    } else if (index == 0) {
      head = head->next;
      delete head->prev;
      head->prev = 0;
    } else if (index == length - 1) {
      tail = tail->prev;
      delete tail->next;
      tail->next = 0;
    } else {
      Node<T> *p = head;
      unsigned int cIndex = 0;

      while (cIndex < index) {
        p = p->next;
        cIndex++;
      }

      p->prev->next = p->next;
      p->next->prev = p->prev;

      delete p;
    }

    length--;
  }

  void free() {
    Node<T> *p = head->next;

    if (length == 0) {
      return;
    }

    while (p) {
      delete p->prev;

      p = p->next;
    }

    delete p;
  }
};

#endif
