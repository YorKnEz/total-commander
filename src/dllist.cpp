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

void updateFilesY(List<File> &files, int y) {
  Node<File> *p = files.head;
  int fileY = y;

  int offsetYText, offsetYIcon;

  if (p) {
    // calculate the offsets for the text and icon
    FloatRect backgroundBounds = p->data.background.getGlobalBounds();
    FloatRect iconBounds = p->data.icon.getGlobalBounds();
    int charSize = p->data.filename.getCharacterSize();

    offsetYText = (backgroundBounds.height - charSize) / 2;
    offsetYIcon = (backgroundBounds.height - iconBounds.height) / 2;
  }

  while (p) {
    p->data.background.setPosition(p->data.background.getPosition().x, fileY);

    p->data.icon.setPosition(p->data.icon.getPosition().x, fileY + offsetYIcon);
    p->data.filename.setPosition(p->data.filename.getPosition().x,
                                 fileY + offsetYText);
    p->data.ext.setPosition(p->data.ext.getPosition().x, fileY + offsetYText);
    p->data.size.setPosition(p->data.size.getPosition().x, fileY + offsetYText);
    p->data.date.setPosition(p->data.date.getPosition().x, fileY + offsetYText);

    fileY += p->data.background.getGlobalBounds().height + 1;

    p = p->next;
  }
}

void updateFilesState(List<File> &files, Node<File> *activeFile[2], Event event,
                      MouseEventType type, Vector2i &oldClick) {
  Node<File> *p = files.head,           // file list iterator
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

void drawFiles(RenderWindow &window, List<File> list, int miny, int maxy) {
  Node<File> *p = list.head;

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

template void init<File>(List<File> &l);
template void init<ColorTheme>(List<ColorTheme> &l);

template void add<File>(List<File> &l, File data, unsigned int index);
template void add<ColorTheme>(List<ColorTheme> &l, ColorTheme data,
                              unsigned int index);

template void remove<File>(List<File> &l, unsigned int index);
template void remove<ColorTheme>(List<ColorTheme> &l, unsigned int index);

template void free<File>(List<File> &l);
template void free<ColorTheme>(List<ColorTheme> &l);
