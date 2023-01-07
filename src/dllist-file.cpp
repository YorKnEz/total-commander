#include "dllist-file.h"

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

// shows in the console the contents of a list
void printList(List<File> l) {
  Node<File> *p = l.head;
  unsigned int index = 0;

  while (p) {
    cout << "Name: " << p->data.data.filename << " | ";
    cout << "Extension: " << p->data.data.ext << " | ";
    cout << "Size: " << p->data.data.size << " | ";
    cout << "Date: " << p->data.data.date << '\n';

    p = p->next;
  }

  cout << '\n';
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

    while (p) {
      p->data.background.setPosition(p->data.background.getPosition().x, fileY);

      p->data.icon.setPosition(p->data.icon.getPosition().x,
                               fileY + offsetYIcon);
      p->data.filename.setPosition(p->data.filename.getPosition().x,
                                   fileY + offsetYText);
      p->data.ext.setPosition(p->data.ext.getPosition().x, fileY + offsetYText);
      p->data.size.setPosition(p->data.size.getPosition().x,
                               fileY + offsetYText);
      p->data.date.setPosition(p->data.date.getPosition().x,
                               fileY + offsetYText);

      if (p->data.data.path.size() > 0) {
        p->data.filename.setPosition(p->data.filename.getPosition().x, fileY + 4);
        p->data.path.setPosition(p->data.filename.getPosition().x,
                                 fileY + backgroundBounds.height - charSize - 4);
      }

      fileY += backgroundBounds.height + 1;

      p = p->next;
    }
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

void updateFilesTheme(List<File> &list,
                      FileStateColors fileStateColors[F_MAX_STATES]) {
  Node<File> *p = list.head;

  while (p) {
    updateFileTheme(p->data, fileStateColors);

    p = p->next;
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
