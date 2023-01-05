#include "dllist.h"

// initializez an empty double linked list
void init(list &l) {
  l.length = 0;
  l.head = l.tail = NULL;
}

// adds an element of type File at index
void add(list &l, File data, unsigned int index) {
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
    cout << "Name: " << p->data.data.filename << " | ";
    cout << "Extension: " << p->data.data.ext << " | ";
    cout << "Size: " << p->data.data.size << " | ";
    cout << "Date: " << p->data.data.date << '\n';

    p = p->next;
  }

  cout << '\n';
}

void free(list &l) {
  // if (l.head) {
  //   node *p = l.head->next;
  //
  //   while (p) {
  //     delete p->prev;
  //
  //     p = p->next;
  //   }
  //
  //   delete p;
  // }

  node *p = l.head;

  while (p) {
    p = p->next;
    remove(l, 0);
  }
  // cout << "\nFreed list\n";
}

void sort(list &l, sortOrder order,
          bool (*sortCriteria)(node *a, node *b, sortOrder order)) {
  if (l.length < 2) {
    return;
  }

  node *p =
      l.head->data.data.size.compare("<DIR>") == 0 ? l.head->next : l.head;
  node *q;
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

void updateFilesY(list &files, int y) {
  node *p = files.head;
  int fileY = y;
  int offsetY = p->data.background.getGlobalBounds().height / 2 -
                p->data.date.getGlobalBounds().height / 2 +
                p->data.date.getPosition().y -
                p->data.date.getGlobalBounds().top;

  while (p) {
    p->data.background.setPosition(p->data.background.getPosition().x, fileY);
    p->data.filename.setPosition(p->data.filename.getPosition().x,
                                 fileY + offsetY);
    p->data.ext.setPosition(p->data.ext.getPosition().x, fileY + offsetY);
    p->data.size.setPosition(p->data.size.getPosition().x, fileY + offsetY);
    p->data.date.setPosition(p->data.date.getPosition().x, fileY + offsetY);

    fileY += p->data.background.getGlobalBounds().height + 1;

    p = p->next;
  }
}

void updateFilesState(list &files, node *activeFile[2], Event event,
                      MouseEventType type, Vector2i &oldClick) {
  node *p = files.head,                 // file list iterator
      *start = nullptr, *end = nullptr; // used for going through the list
  Keyboard kbd;                         // used for checking combos
  bool shouldSelect = false;            // flag used for checking reselect

  while (p) {
    switch (type) {
    case DCLICK: // dclick is handled only if it's a normal click
    case CLICK:
      // if there is a click on a file
      if (isHovered(p->data.background.getGlobalBounds(), event.mouseButton.x,
                    event.mouseButton.y)) {
        // if user presses control, toggle the select state of clicked file
        if (kbd.isKeyPressed(Keyboard::LControl)) {
          activeFile[0] = p;
          activeFile[0]->data.state =
              activeFile[0]->data.state == F_SELECTED ? F_INACTIVE : F_SELECTED;
        }
        // if no file has been selected, set first file as crt file
        else if (!activeFile[0]) {
          activeFile[0] = p;
          activeFile[0]->data.state = F_SELECTED;
          shouldSelect = true;
        }
        // if the first file has been selected, select the second
        else {
          // select the second if shift key is also pressed
          if (kbd.isKeyPressed(Keyboard::LShift) && activeFile[0] != p) {
            // select the new one
            activeFile[1] = p;
            activeFile[1]->data.state = F_SELECTED;
          }
          // select the first if the user just clicked
          else {
            activeFile[0]->data.state = F_INACTIVE;
            activeFile[0] = p;
            activeFile[0]->data.state = F_SELECTED;

            if (activeFile[1]) {
              activeFile[1]->data.state = F_INACTIVE;
              activeFile[1] = nullptr;
            }
          }
          shouldSelect = true;
        }

        // if a click happened, we should probably redo the selection
      }
      break;
    }

    p = p->next;
  }

  if (shouldSelect) {
    if (!activeFile[1]) {
      p = files.head; // get current file pointer

      // deselect all files that are not the currently select
      while (p) {
        if (p != activeFile[0]) {
          p->data.state = F_INACTIVE;
        }

        p = p->next;
      }
    } else {
      // init start and end pointers
      start = activeFile[0];
      end = activeFile[1];

      // swap the pointers if they are not in order
      if (start->data.background.getPosition().y >
          end->data.background.getPosition().y) {
        swap(start, end);
      }

      // make the files selected in case they weren't
      start->data.state = F_SELECTED;
      end->data.state = F_SELECTED;

      // select all files between start and end
      while (start != end) {
        start->data.state = F_SELECTED;

        start = start->next;
      }
    }
  }
}

void drawFiles(RenderWindow &window, list list, int miny, int maxy) {
  node *p = list.head;

  while (p) {
    FloatRect currentFileBounds = p->data.background.getGlobalBounds();

    // draw only the files that are on the screen
    if (currentFileBounds.top + currentFileBounds.height > miny &&
        currentFileBounds.top < maxy) {
      drawFile(window, p->data);
    }

    // break the loop if the current file is below the screen
    if (currentFileBounds.top >= maxy) {
      break;
    }

    p = p->next;
  }
}
