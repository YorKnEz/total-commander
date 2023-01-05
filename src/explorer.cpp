#include "explorer.h"

Explorer createExplorer(string path, Font &font, int charSize, int x, int y,
                        int width, int height, ColorTheme theme) {
  Explorer explorer;

  int scrollbarWidth = 20;
  explorer.heightFile = 27; // used for files and file sorting buttons
  explorer.heightComp = 30; // used for anything else

  explorer.path = path; // set path
  explorer.scrollOffset = 0;

  // initialize the background
  explorer.background.setSize(Vector2f(width, height));
  explorer.background.setFillColor(theme.bgBody);
  explorer.background.setPosition(x, y);

  // initialize the files list
  init(explorer.files);

  getFilesFromPath(explorer.files, path, font, charSize, x,
                   y + 2 * explorer.heightComp + explorer.heightFile,
                   width - scrollbarWidth - 2, explorer.heightFile,
                   theme.fileStateColors);

  sortFiletree(explorer.files, explorer.sortedBy, explorer.order);

  updateFilesY(explorer.files, y + 2 * explorer.heightComp +
                                   explorer.heightFile + explorer.scrollOffset);

  Node<File> *head = explorer.files.head; // head of the file list

  int nameX = x + 1, extX = nameX + head->data.filenameColumn,
      sizeX = extX + head->data.extColumn,
      dateX =
          sizeX +
          head->data.sizeColumn; // used for setting the x of the file columns
  int btnY = y + 2 * explorer.heightComp +
             1; // used for setting the y of the file columns

  // set the drive size text box
  explorer.textbox[0] = createTextBox(
      getSizeOfDrive(explorer.path), font, charSize, x + 1, y + 1, width - 2,
      explorer.heightComp - 2, theme.textMediumContrast, theme.bgLowContrast,
      theme.border, 1);

  // set the input
  explorer.input =
      createInput("Enter path here", path, font, charSize, x + 1,
                  y + explorer.heightComp + 1, width - 2,
                  explorer.heightComp - 2, theme.inputStateColors, 1);

  // set the sorting buttons
  explorer.button[0] = createButton(
      "Name", font, charSize, nameX, btnY, head->data.filenameColumn - 2,
      explorer.heightFile - 2, theme.buttonStateColors, 1);
  explorer.button[1] =
      createButton("Ext", font, charSize, extX, btnY, head->data.extColumn - 2,
                   explorer.heightFile - 2, theme.buttonStateColors, 1);
  explorer.button[2] = createButton(
      "Size", font, charSize, sizeX, btnY, head->data.sizeColumn - 2,
      explorer.heightFile - 2, theme.buttonStateColors, 1);
  explorer.button[3] = createButton(
      "Date", font, charSize, dateX, btnY, head->data.dateColumn - 1,
      explorer.heightFile - 2, theme.buttonStateColors, 1);

  // append the sort indicator to the new sort button
  explorer.button[explorer.sortedBy].fullText.append(
      explorer.order == ASC ? " /" : " \\");
  updateText(explorer.button[explorer.sortedBy].text,
             explorer.button[explorer.sortedBy].fullText,
             explorer.button[explorer.sortedBy].background.getGlobalBounds());

  // set the current folder text box
  explorer.textbox[1] = createTextBox(
      getCurrentFolder(explorer.path), font, charSize, x + 1,
      y + height - explorer.heightComp + 1, width - 2, explorer.heightComp - 2,
      theme.textMediumContrast, theme.bgLowContrast, theme.border, 1);

  explorer.scrollbar = createScrollbar(
      font, charSize, x + width - scrollbarWidth - 2, btnY, scrollbarWidth + 1,
      height - 3 * explorer.heightComp - 4, explorer.heightFile - 2,
      (explorer.heightFile + 1) * explorer.files.length,
      theme.buttonStateColors, 1);

  return explorer;
}

void refreshExplorer(Explorer &explorer, Explorer *activeExplorer, Font &font,
                     ColorTheme theme) {
  // use the head of the old list to extract its props
  File file = explorer.files.head->data;

  // update the input of the explorer
  explorer.input.value = explorer.path;
  explorer.input.displayText.setString(explorer.path);
  explorer.input.displayLength = explorer.path.size();
  explorer.input.cursorLocation = explorer.path.size();
  explorer.input.startPosition = 0;

  shrinkInput(explorer.input);

  // reset explorer drive size
  explorer.textbox[0].fullText = getSizeOfDrive(explorer.path);
  updateText(explorer.textbox[0].text, explorer.textbox[0].fullText,
             explorer.textbox[0].background.getGlobalBounds());

  // reset explorer related props
  explorer.textbox[1].fullText = getCurrentFolder(explorer.path);
  if (activeExplorer == &explorer) {
    explorer.textbox[1].fullText.insert(0, "> ");
    explorer.textbox[1].fullText.append(" <");
    updateText(explorer.textbox[1].text, explorer.textbox[1].fullText,
               explorer.textbox[1].background.getGlobalBounds());
  }

  // delete old files list
  free(explorer.files);
  init(explorer.files);

  // get the new files from the new path
  getFilesFromPath(
      explorer.files, explorer.path, font, file.filename.getCharacterSize(),
      file.background.getPosition().x, file.background.getPosition().y,
      file.background.getGlobalBounds().width, explorer.heightFile,
      theme.fileStateColors);

  // re-sort and update the y of the files
  sortFiletree(explorer.files, explorer.sortedBy, explorer.order);

  updateFilesY(explorer.files,
               file.background.getPosition().y - explorer.scrollOffset);

  // reset active file pointers
  explorer.activeFile[0] = explorer.files.head;
  explorer.activeFile[0]->data.state = F_SELECTED;
  explorer.activeFile[1] = nullptr;

  // update scrollbar
  updateScrollbar(explorer.scrollbar, 0); // reset scrollbar to offset 0
  updateScrollableHeight(explorer.scrollbar,
                         (explorer.heightFile + 1) * explorer.files.length);

  explorer.scrollOffset = 0;           // reset scroll offset
  explorer.scrollbar.scrollOffset = 0; // reset scroll offset of scrollbar
}

void updateFilesY(List<File> &files, int y) {
  Node<File> *p = files.head;
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

void updateFilesState(Explorer &explorer, Event event, MouseEventType type,
                      Vector2i &oldClick) {
  Node<File> *p = explorer.files.head,  // file list iterator
      *start = nullptr, *end = nullptr; // used for going through the list
  Keyboard kbd;                         // used for checking combos
  bool shouldSelect = false;            // flag used for checking reselect

  while (p) {
    switch (type) {
    case DCLICK:
      // if a dclick happens, let the explorer handle it
      if (explorer.activeFile[0] == p) {
        break;
      }
    case CLICK:
      // if there is a click on a file
      if (isHovered(p->data.background.getGlobalBounds(), event.mouseButton.x,
                    event.mouseButton.y)) {
        // if user presses control, toggle the select state of clicked file
        if (kbd.isKeyPressed(Keyboard::LControl)) {
          explorer.activeFile[0] = p;
          explorer.activeFile[0]->data.state =
              explorer.activeFile[0]->data.state == F_SELECTED ? F_INACTIVE
                                                               : F_SELECTED;
        }
        // if no file has been selected, set first file as crt file
        else if (!explorer.activeFile[0]) {
          explorer.activeFile[0] = p;
          explorer.activeFile[0]->data.state = F_SELECTED;
          shouldSelect = true;
        }
        // if the first file has been selected, select the second
        else {
          // select the second if shift key is also pressed
          if (kbd.isKeyPressed(Keyboard::LShift) &&
              explorer.activeFile[0] != p) {
            // select the new one
            explorer.activeFile[1] = p;
            explorer.activeFile[1]->data.state = F_SELECTED;
          }
          // select the first if the user just clicked
          else {
            explorer.activeFile[0]->data.state = F_INACTIVE;
            explorer.activeFile[0] = p;
            explorer.activeFile[0]->data.state = F_SELECTED;

            if (explorer.activeFile[1]) {
              explorer.activeFile[1]->data.state = F_INACTIVE;
              explorer.activeFile[1] = nullptr;
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
    if (!explorer.activeFile[1]) {
      p = explorer.files.head; // get current file pointer

      // deselect all files that are not the currently select
      while (p) {
        if (p != explorer.activeFile[0]) {
          p->data.state = F_INACTIVE;
        }

        p = p->next;
      }
    } else {
      // init start and end pointers
      start = explorer.activeFile[0];
      end = explorer.activeFile[1];

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

void updateScrollbarState(Explorer &explorer, Event event, MouseEventType type,
                          Vector2i &oldClick) {
  // update scrollbar buttons
  updateButtonState(explorer.scrollbar.up, event, type, oldClick);
  updateButtonState(explorer.scrollbar.down, event, type, oldClick);

  if (explorer.scrollbar.up.state == B_CLICKED ||
      explorer.scrollbar.up.state == B_DCLICKED) {
    scrollFiles(&explorer, UP);
    // turn off the button to avoid side effects (such as the event triggering
    // multipel times)
    explorer.scrollbar.up.state = B_INACTIVE;
  } else if (explorer.scrollbar.down.state == B_CLICKED ||
             explorer.scrollbar.down.state == B_DCLICKED) {
    scrollFiles(&explorer, DOWN);
    // turn off the button to avoid side effects (such as the event triggering
    // multipel times)
    explorer.scrollbar.down.state = B_INACTIVE;
  }

  switch (type) {
  case RELEASE:
    explorer.scrollbar.state = B_INACTIVE;
    break;
  case DCLICK:
  case CLICK:
    if (isHovered(explorer.scrollbar.thumb.getGlobalBounds(),
                  event.mouseButton.x, event.mouseButton.y)) {
      explorer.scrollbar.state = B_CLICKED;
      explorer.scrollbar.oldMouseY = event.mouseButton.y;
      oldClick = Vector2i(event.mouseButton.x, event.mouseButton.y);
    }
    break;
  case MOVE:
    if (explorer.scrollbar.state == B_CLICKED) {
      // get the offset by which the cursor has moved relative to the last
      // move position
      int newOffset = (event.mouseMove.y - explorer.scrollbar.oldMouseY);
      // update the old scroll offset
      explorer.scrollbar.oldMouseY = event.mouseMove.y;
      // ratio the new offset to be used for scrolling the files
      newOffset = float(newOffset) / (-getScrollbarRatio(explorer.scrollbar));

      Direction d = Direction(newOffset);

      scrollFiles(&explorer, d);
    }

    break;
  }
}

void updateExplorerState(Explorer &explorer, Event event, MouseEventType type,
                         Explorer *&activeExplorer, Vector2i &oldClick,
                         Input *&activeInput, Font &font, ColorTheme theme) {
  switch (type) {
  case CLICK:
    // if the user clicks inside the explorer, move the focus to the explorer
    if (isHovered(explorer.background.getGlobalBounds(), event.mouseButton.x,
                  event.mouseButton.y) &&
        activeExplorer != &explorer) {
      if (activeExplorer) {
        activeExplorer->state = E_INACTIVE; // set old explorer as inactive

        // remove the focused explorer indicator
        activeExplorer->textbox[1].fullText.erase(0, 2);
        activeExplorer->textbox[1].fullText.erase(
            activeExplorer->textbox[1].fullText.size() - 2, 2);

        updateText(activeExplorer->textbox[1].text,
                   activeExplorer->textbox[1].fullText,
                   activeExplorer->textbox[1].background.getGlobalBounds());
      }

      activeExplorer = &explorer;
      activeExplorer->state = E_ACTIVE; // set current explorer as active

      // add the focused explorer indicator
      activeExplorer->textbox[1].fullText.insert(0, "> ");
      activeExplorer->textbox[1].fullText.append(" <");

      updateText(activeExplorer->textbox[1].text,
                 activeExplorer->textbox[1].fullText,
                 activeExplorer->textbox[1].background.getGlobalBounds());
    }
    break;
  }

  // update the state of the buttons
  for (int i = 0; i < 4; i++) {
    updateButtonState(explorer.button[i], event, type, oldClick);

    if (explorer.button[i].state == B_CLICKED ||
        explorer.button[i].state == B_DCLICKED) {
      sortFiles(explorer, sortBy(i));

      // turn off the button to avoid side effects (such as the event triggering
      // multipel times)
      explorer.button[i].state = B_INACTIVE;
    }
  }

  updateScrollbarState(explorer, event, type, oldClick);

  FloatRect filelistBounds = explorer.background.getGlobalBounds();
  filelistBounds.top += 2 * explorer.heightComp + explorer.heightFile;
  filelistBounds.height -= (3 * explorer.heightComp + explorer.heightFile);

  if (isHovered(filelistBounds, event.mouseButton.x, event.mouseButton.y)) {
    if (type == DCLICK && explorer.activeFile[0] &&
        isHovered(explorer.activeFile[0]->data.background.getGlobalBounds(),
                  event.mouseButton.x, event.mouseButton.y)) {
      if (explorer.activeFile[0]->data.data.size == "<DIR>") {
        openFolder(explorer.path, explorer.activeFile[0]->data.data.filename);

        refreshExplorer(explorer, activeExplorer, font, theme);
      } else {
        openFile(explorer.path, explorer.activeFile[0]->data.data.filename,
                 explorer.activeFile[0]->data.data.ext);
      }
    } else {
      updateFilesState(explorer, event, type, oldClick);
    }
  }

  // update the state of the input
  updateInputState(explorer.input, event, type, activeInput);
}

void sortFiles(Explorer &explorer, sortBy criteria) {
  // remove the sort indicator from the last button
  explorer.button[explorer.sortedBy].fullText.erase(
      explorer.button[explorer.sortedBy].fullText.size() - 2);
  updateText(explorer.button[explorer.sortedBy].text,
             explorer.button[explorer.sortedBy].fullText,
             explorer.button[explorer.sortedBy].background.getGlobalBounds());

  // update sortedBy and order indicators
  if (explorer.sortedBy == criteria) {
    explorer.order = explorer.order == ASC ? DESC : ASC;
  } else {
    explorer.order = ASC;
  }
  explorer.sortedBy = criteria;

  // append the sort indicator to the new sort button
  explorer.button[explorer.sortedBy].fullText.append(
      explorer.order == ASC ? " /" : " \\");
  updateText(explorer.button[criteria].text, explorer.button[criteria].fullText,
             explorer.button[criteria].background.getGlobalBounds());

  // sort and update the y of the files
  sortFiletree(explorer.files, explorer.sortedBy, explorer.order);
  updateFilesY(explorer.files,
               explorer.background.getPosition().y + explorer.heightFile +
                   2 * explorer.heightComp + explorer.scrollOffset);
}

void scrollFiles(Explorer *activeExplorer, Direction d) {
  // check if file list is scrollable
  if (activeExplorer->scrollbar.thumb.getGlobalBounds().height <
      activeExplorer->scrollbar.track.getGlobalBounds().height) {
    // scroll up means the content moves down and the scollbar up
    activeExplorer->scrollOffset += int(d);

    int displayHeight = activeExplorer->background.getGlobalBounds().height -
                        3 * activeExplorer->heightComp -
                        activeExplorer->heightFile;

    if (activeExplorer->scrollOffset > 0) {
      activeExplorer->scrollOffset = 0;
    } else if (activeExplorer->scrollOffset <
               displayHeight - activeExplorer->scrollbar.scrollableHeight) {
      activeExplorer->scrollOffset =
          displayHeight - activeExplorer->scrollbar.scrollableHeight;
    }

    updateFilesY(activeExplorer->files,
                 activeExplorer->background.getPosition().y +
                     activeExplorer->heightFile +
                     2 * activeExplorer->heightComp +
                     activeExplorer->scrollOffset);
    updateScrollbar(activeExplorer->scrollbar, activeExplorer->scrollOffset);
  }
}

void drawFiles(RenderWindow &window, Explorer explorer) {
  Node<File> *p = explorer.files.head;

  FloatRect explorerBounds = explorer.background.getGlobalBounds(),
            currentFileBounds;
  // the y coordonates for which files can be drawn in between
  int miny = explorerBounds.top + 2 * explorer.heightComp + explorer.heightFile,
      maxy = explorerBounds.top + explorerBounds.height - explorer.heightComp;

  while (p) {
    currentFileBounds = p->data.background.getGlobalBounds();

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

void drawExplorer(RenderWindow &window, Explorer explorer) {
  window.draw(explorer.background);

  drawFiles(window, explorer);

  for (int i = 0; i < 4; i++) {
    drawButton(window, explorer.button[i]);
  }

  drawInput(window, explorer.input);

  for (int i = 0; i < 2; i++) {
    drawTextBox(window, explorer.textbox[i]);
  }

  drawScrollbar(window, explorer.scrollbar);
}

void closeExplorer(Explorer &explorer) { free(explorer.files); }
