#include "dllist.h"

// initializez an empty double linked list
template <typename T> void init(List<T> &l) {
  l.length = 0;
  l.head = l.tail = NULL;
}

// adds an element of type File at index
template <typename T> void add(List<T> &l, T data, unsigned int index) {
  if (index > l.length)
    return;

  Node<T> *n = new Node<T>;
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
    Node<T> *p = l.head;
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
template <typename T> void remove(List<T> &l, unsigned int index) {
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
    Node<T> *p = l.head;
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
void printList(List<File> l) {
  Node<File> *p = l.head;
  unsigned int index = 0;

  while (p != NULL) {
    cout << "Name: " << p->data.data.filename << " | ";
    cout << "Extension: " << p->data.data.ext << " | ";
    cout << "Size: " << p->data.data.size << " | ";
    cout << "Date: " << p->data.data.date << '\n';

    p = p->next;
  }

  cout << '\n';
}

void printList(List<ColorTheme> l) {
  Node<ColorTheme> *p = l.head;
  unsigned int index = 0;

  while (p != NULL) {
    cout << "Text High Contrast: " << std::hex
         << p->data.textHighContrast.toInteger() << "\n";
    cout << "Text Medium Contrast: " << std::hex
         << p->data.textMediumContrast.toInteger() << "\n";
    cout << "Text Low Contrast: " << std::hex
         << p->data.textLowContrast.toInteger() << "\n\n";

    cout << "Background Body : " << std::hex << p->data.bgBody.toInteger()
         << "\n";
    cout << "Background Low Contrast: " << std::hex
         << p->data.bgLowContrast.toInteger() << "\n\n";

    cout << "Border: " << std::hex << p->data.border.toInteger() << "\n\n";

    for (int i = 0; i < B_MAX_STATES; i++) {
      cout << "Button State " << i << " Colors " << std::hex
           << p->data.buttonStateColors[i].text.toInteger() << " | ";
      cout << std::hex << p->data.buttonStateColors[i].background.toInteger()
           << " | ";
      cout << std::hex << p->data.buttonStateColors[i].border.toInteger()
           << "\n";
    }

    cout << "\n";

    for (int i = 0; i < F_MAX_STATES; i++) {
      cout << "Input State " << i << " Colors " << std::hex
           << p->data.fileStateColors[i].textHighContrast.toInteger() << " | ";
      cout << std::hex << p->data.fileStateColors[i].textLowContrast.toInteger()
           << " | ";
      cout << std::hex << p->data.fileStateColors[i].background.toInteger()
           << " | ";
      cout << std::hex << p->data.fileStateColors[i].border.toInteger() << "\n";
    }

    cout << "\n";

    for (int i = 0; i < I_MAX_STATES; i++) {
      cout << "Input State " << i << " Colors " << std::hex
           << p->data.inputStateColors[i].text.toInteger() << " | ";
      cout << std::hex << p->data.inputStateColors[i].background.toInteger()
           << " | ";
      cout << std::hex << p->data.inputStateColors[i].border.toInteger()
           << "\n";
    }

    cout << "\n";

    p = p->next;
  }
}

template <typename T> void free(List<T> &l) {
  Node<T> *p = l.head;

  while (p) {
    p = p->next;
    remove(l, 0);
  }
}

void sort(List<File> &l, sortOrder order,
          bool (*sortCriteria)(Node<File> *a, Node<File> *b, sortOrder order)) {
  if (l.length < 2) {
    return;
  }

  Node<File> *p =
      l.head->data.data.size.compare("<DIR>") == 0 ? l.head->next : l.head;
  Node<File> *q;
  File aux;

  while (p->next && !p->data.data.size.compare("<DIR>")) {
    q = p->next;

    while (q && !q->data.data.size.compare("<DIR>")) {
      if (sortCriteria(q, p, order)) {
        swap(p->data, q->data);
      }

      q = q->next;
    }

    p = p->next;
  }

  while (p->next) {
    q = p->next;

    while (q) {
      if (sortCriteria(q, p, order)) {
        swap(p->data, q->data);
      }

      q = q->next;
    }

    p = p->next;
  }
}

template void init<File>(List<File> &l);
template void init<ColorTheme>(List<ColorTheme> &l);

template void add<File>(List<File> &l, File data, unsigned int index);
template void add<ColorTheme>(List<ColorTheme> &l, ColorTheme data,
                              unsigned int index);

template void remove<File>(List<File> &l, unsigned int index);
template void remove<ColorTheme>(List<ColorTheme> &l, unsigned int index);

template void free<File>(List<File> &l);
template void free<ColorTheme>(List<ColorTheme> &l);