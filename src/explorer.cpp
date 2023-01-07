#include "explorer.h"

void sortFiles(Explorer &explorer, sortBy criteria, Texture *sortIndicatorAsc,
               Texture *sortIndicatorDesc) {
  // remove the sort indicator from the last button
  updateButtonIcon(explorer.button[explorer.sortedBy]);

  // update sortedBy and order indicators
  if (explorer.sortedBy == criteria) {
    explorer.order = explorer.order == ASC ? DESC : ASC;
  } else {
    explorer.order = ASC;
  }
  explorer.sortedBy = criteria;

  // add the sort indicator to the new sort buton
  updateButtonIcon(explorer.button[explorer.sortedBy], explorer.order == ASC
                                                           ? sortIndicatorAsc
                                                           : sortIndicatorDesc);

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

    // get the start position of the files
    int y = activeExplorer->background.getPosition().y +
            activeExplorer->heightFile + 2 * activeExplorer->heightComp +
            activeExplorer->scrollOffset;

    if (activeExplorer->forestView) {
      updateFileForestXY(activeExplorer->fileForest,
                         activeExplorer->background.getPosition().x, y);
    } else {
      updateFilesY(activeExplorer->files, y);
    }

    updateScrollbar(activeExplorer->scrollbar, activeExplorer->scrollOffset);
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

Explorer createExplorer(string path, int x, int y, int width, int height,
                        Theme &theme) {
  Explorer explorer;

  int scrollbarWidth = 20;
  explorer.heightFile = 27; // used for files and file sorting buttons
  explorer.heightComp = 30; // used for anything else

  explorer.path = path; // set path
  explorer.scrollOffset = 0;

  // initialize the background
  explorer.background.setSize(Vector2f(width, height));
  explorer.background.setFillColor(theme.colors.bgBody);
  explorer.background.setPosition(x, y);

  explorer.forestView = false;

  // initialize the files list
  explorer.files.init();

  getFilesFromPath(explorer.files, path, x,
                   y + 2 * explorer.heightComp + explorer.heightFile,
                   width - scrollbarWidth - 2, explorer.heightFile, theme);

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
      getSizeOfDrive(explorer.path), theme.font, theme.charSize, x + 1, y + 1,
      width - 2, explorer.heightComp - 2, theme.colors.textMediumContrast,
      theme.colors.bgLowContrast, theme.colors.border, 1);

  // set the input
  explorer.input =
      createInput("Enter path here", path, theme.font, theme.charSize, x + 1,
                  y + explorer.heightComp + 1, width - 2,
                  explorer.heightComp - 2, theme.colors.inputStateColors, 1);

  // set the sorting buttons
  explorer.button[0] =
      createButton("Name", theme.font, theme.charSize, nameX, btnY,
                   head->data.filenameColumn - 2, explorer.heightFile - 2,
                   theme.colors.buttonStateColors, 1, &theme.upArrow);
  explorer.button[1] = createButton(
      "Ext", theme.font, theme.charSize, extX, btnY, head->data.extColumn - 2,
      explorer.heightFile - 2, theme.colors.buttonStateColors, 1);
  explorer.button[2] =
      createButton("Size", theme.font, theme.charSize, sizeX, btnY,
                   head->data.sizeColumn - 2, explorer.heightFile - 2,
                   theme.colors.buttonStateColors, 1);
  explorer.button[3] =
      createButton("Date", theme.font, theme.charSize, dateX, btnY,
                   head->data.dateColumn - 2, explorer.heightFile - 2,
                   theme.colors.buttonStateColors, 1);
  // set the toggle forest view button
  explorer.button[4] =
      createButton("", theme.font, theme.charSize,
                   x + width - explorer.heightComp + 1,
                   y + height - explorer.heightComp + 1,
                   explorer.heightComp - 2, explorer.heightComp - 2,
                   theme.colors.buttonStateColors, 1, &theme.diagram);

  // set the current folder text box
  explorer.textbox[1] =
      createTextBox(getCurrentFolder(explorer.path), theme.font, theme.charSize,
                    x + 1, y + height - explorer.heightComp + 1, width - 2,
                    explorer.heightComp - 2, theme.colors.textMediumContrast,
                    theme.colors.bgLowContrast, theme.colors.border, 1);

  int scrollableHeight = (explorer.heightFile + 1) * explorer.files.length;

  explorer.scrollbar = createScrollbar(
      &theme.upArrow, &theme.downArrow, theme.font, theme.charSize,
      x + width - scrollbarWidth - 2, btnY, scrollbarWidth + 1,
      height - 3 * explorer.heightComp - 4, explorer.heightFile - 2,
      scrollableHeight, theme.colors.buttonStateColors, 1);

  return explorer;
}

void refreshExplorer(Explorer &explorer, Explorer *activeExplorer,
                     Theme &theme) {
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

  // refresh file list
  // use the head of the old list to extract its props
  File file = explorer.files.head->data;

  // get the bounds of the area in which files can be drawn
  FloatRect filelistBounds = explorer.background.getGlobalBounds();
  filelistBounds.top += 2 * explorer.heightComp + explorer.heightFile;
  filelistBounds.height -= (3 * explorer.heightComp + explorer.heightFile);

  // get the position from which to start drawing the files
  int y = filelistBounds.top;

  // delete old files list
  explorer.files.free();
  explorer.files.init();

  // get the new files from the new path
  getFilesFromPath(
      explorer.files, explorer.path, file.background.getPosition().x,
      file.background.getPosition().y, file.background.getGlobalBounds().width,
      explorer.heightFile, theme);

  // re-sort and update the y of the files
  sortFiletree(explorer.files, explorer.sortedBy, explorer.order);

  if (explorer.forestView) {
    Node<File> *oldHead = explorer.files.head;
    explorer.files.head = oldHead->next;
    explorer.files.length--;

    init(explorer.fileForest, explorer.files,
         explorer.files.head->data.background.getGlobalBounds().width);

    explorer.files.head = oldHead;
    explorer.files.length++;

    // get scrollable height
    int scrollableHeight =
        getFileForestHeight(explorer.fileForest, explorer.heightFile);

    // update the scrollable height
    updateScrollbar(explorer.scrollbar, 0); // reset scrollbar to offset 0
    updateScrollableHeight(explorer.scrollbar, scrollableHeight);

    // update the coords of the files drawn on the screen
    updateFileForestXY(explorer.fileForest, filelistBounds.left, y);
  } else {

    // reset active file pointers
    explorer.activeFile[0] = explorer.files.head;
    explorer.activeFile[0]->data.state = F_SELECTED;
    explorer.activeFile[1] = nullptr;

    // get scrollable height
    int scrollableHeight = (explorer.heightFile + 1) * explorer.files.length;

    // update the scrollable height
    updateScrollbar(explorer.scrollbar, 0); // reset scrollbar to offset 0
    updateScrollableHeight(explorer.scrollbar, scrollableHeight);

    // update the coords of the files draw on the screen
    updateFilesY(explorer.files, y);
  }

  explorer.scrollOffset = 0; // reset scroll offset
}

void updateExplorerIndicator(Explorer *explorer, Explorer *&activeExplorer) {
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

  activeExplorer = explorer;
  activeExplorer->state = E_ACTIVE; // set current explorer as active

  // add the focused explorer indicator
  activeExplorer->textbox[1].fullText.insert(0, "> ");
  activeExplorer->textbox[1].fullText.append(" <");

  updateText(activeExplorer->textbox[1].text,
             activeExplorer->textbox[1].fullText,
             activeExplorer->textbox[1].background.getGlobalBounds());
}

void updateExplorerState(Explorer &explorer, Event event, MouseEventType type,
                         Explorer *&activeExplorer, Vector2i &oldClick,
                         Input *&activeInput, Theme &theme) {
  switch (type) {
  case CLICK:
    // if the user clicks inside the explorer, move the focus to the explorer
    if (isHovered(explorer.background.getGlobalBounds(), event.mouseButton.x,
                  event.mouseButton.y) &&
        activeExplorer != &explorer) {
      updateExplorerIndicator(&explorer, activeExplorer);
    }
    break;
  }

  // update the state of the sort buttons
  for (int i = 0; i < 4; i++) {
    updateButtonState(explorer.button[i], event, type, oldClick);

    if (explorer.button[i].state == B_CLICKED ||
        explorer.button[i].state == B_DCLICKED) {
      sortFiles(explorer, sortBy(i), &theme.upArrow, &theme.downArrow);

      // turn off the button to avoid side effects (such as the event triggering
      // multipel times)
      explorer.button[i].state = B_INACTIVE;
    }
  }

  // update the state of the other buttons
  updateButtonState(explorer.button[4], event, type, oldClick);

  if (explorer.button[4].state == B_CLICKED ||
      explorer.button[4].state == B_DCLICKED) {
    explorer.forestView = !explorer.forestView;

    // initialize the file forest
    if (explorer.forestView) {
      Node<File> *oldHead = explorer.files.head;
      explorer.files.head = oldHead->next;
      explorer.files.length--;

      init(explorer.fileForest, explorer.files,
           explorer.files.head->data.background.getGlobalBounds().width);

      explorer.files.head = oldHead;
      explorer.files.length++;

      // get scrollable height
      int scrollableHeight =
          getFileForestHeight(explorer.fileForest, explorer.heightFile);

      // update the scrollable height
      updateScrollbar(explorer.scrollbar, 0); // reset scrollbar to offset 0
      updateScrollableHeight(explorer.scrollbar, scrollableHeight);

      // update the coords of the files draw on the screen
      int x = explorer.files.head->data.background.getPosition().x;
      int y = explorer.files.head->data.background.getPosition().y;

      updateFileForestXY(explorer.fileForest, x, y);
    }
    // free the file forest and restore the file list
    else {
      free(explorer.fileForest);

      // update the scrollable height
      int scrollableHeight = (explorer.heightFile + 1) * explorer.files.length;

      // update the scrollable height
      updateScrollbar(explorer.scrollbar, 0); // reset scrollbar to offset 0
      updateScrollableHeight(explorer.scrollbar, scrollableHeight);

      // update the coords of the files draw on the screen
      int y = explorer.files.head->data.background.getPosition().y;

      updateFilesY(explorer.files, y);
    }

    explorer.button[4].state = B_INACTIVE;
  }

  updateScrollbarState(explorer, event, type, oldClick);

  FloatRect filelistBounds = explorer.background.getGlobalBounds();
  filelistBounds.top += 2 * explorer.heightComp + explorer.heightFile;
  filelistBounds.height -= (3 * explorer.heightComp + explorer.heightFile);

  // update files state if the click is in the file list bounds
  if (isHovered(filelistBounds, event.mouseButton.x, event.mouseButton.y)) {
    if (explorer.forestView) {
      if (type == DCLICK && explorer.activeForest &&
          isHovered(explorer.activeForest->data.background.getGlobalBounds(),
                    event.mouseButton.x, event.mouseButton.y)) {
        if (explorer.activeForest->data.data.size == "<DIR>") {
          // if the forest is empty, add its contents
          if (explorer.activeForest->length == 0) {
            List<File> newFiles; // for storing the content of the opened folder
            newFiles.init();

            File file = explorer.fileForest.files[0].data;

            string newFilesPath = explorer.activeForestPath + SEP +
                                  explorer.activeForest->data.data.filename;

            bool ignoreBackwardsFolder = true;

            // get the new files from the new path
            getFilesFromPath(newFiles, newFilesPath,
                             file.background.getPosition().x,
                             file.background.getPosition().y,
                             file.background.getGlobalBounds().width,
                             explorer.heightFile, theme, ignoreBackwardsFolder);

            // re-sort and update the y of the files
            sortFiletree(newFiles, explorer.sortedBy, explorer.order);

            init(
                *explorer.activeForest, newFiles,
                explorer.activeForest->data.background.getGlobalBounds().width -
                    20);
          }
          // else toggle the hidden state
          else {
            explorer.activeForest->hidden = !explorer.activeForest->hidden;
          }

          // get scrollable height
          int scrollableHeight =
              getFileForestHeight(explorer.fileForest, explorer.heightFile);

          // update the scrollable height
          updateScrollableHeight(explorer.scrollbar, scrollableHeight);

          if (scrollableHeight < filelistBounds.height) {
            int y = filelistBounds.top;

            // reset the position of the file forest
            updateFileForestXY(activeExplorer->fileForest,
                               activeExplorer->background.getPosition().x, y);

            // reset the scrollbar position
            updateScrollbar(explorer.scrollbar, 0);
          } else {
            scrollFiles(&explorer, Direction(0));
          }
        } else {
          openFile(explorer.activeForestPath,
                   explorer.activeForest->data.data.filename,
                   explorer.activeForest->data.data.ext);
        }
      } else {
        updateFileForestState(explorer.fileForest, explorer.activeForestPath,
                              explorer.activeForest, explorer.path, event, type,
                              oldClick);
      }
    } else {
      // open currently dclicked file
      if (type == DCLICK && explorer.activeFile[0] &&
          isHovered(explorer.activeFile[0]->data.background.getGlobalBounds(),
                    event.mouseButton.x, event.mouseButton.y)) {
        if (explorer.activeFile[0]->data.data.size == "<DIR>") {
          openFolder(explorer.path, explorer.activeFile[0]->data.data.filename);

          refreshExplorer(explorer, activeExplorer, theme);
        } else {
          openFile(explorer.path, explorer.activeFile[0]->data.data.filename,
                   explorer.activeFile[0]->data.data.ext);
        }
      } else {
        updateFilesState(explorer.files, explorer.activeFile, event, type,
                         oldClick);
      }
    }
  }

  // update the state of the input
  updateInputState(explorer.input, event, type, activeInput);
}

void updateExplorerTheme(Explorer &explorer, Theme &theme) {
  explorer.background.setFillColor(theme.colors.bgBody);

  for (int i = 0; i < 2; i++) {
    updateTextBoxTheme(explorer.textbox[i], theme.colors.textMediumContrast,
                       theme.colors.bgLowContrast, theme.colors.border);
  }

  updateInputTheme(explorer.input, theme.colors.inputStateColors);

  updateFilesTheme(explorer.files, theme.colors.fileStateColors);

  if (explorer.forestView) {
    updateFileForestTheme(explorer.fileForest, theme.colors.fileStateColors);
  }

  for (int i = 0; i < 5; i++) {
    updateButtonTheme(explorer.button[i], theme.colors.buttonStateColors);
  }

  updateScrollbarTheme(explorer.scrollbar, theme.colors.buttonStateColors);
}

void drawExplorer(RenderWindow &window, Explorer explorer) {
  window.draw(explorer.background);

  FloatRect explorerBounds = explorer.background.getGlobalBounds();

  // the y coordonates for which files can be drawn in between
  int miny = explorerBounds.top + 2 * explorer.heightComp + explorer.heightFile,
      maxy = explorerBounds.top + explorerBounds.height - explorer.heightComp;

  if (explorer.forestView) {
    drawFileForest(window, explorer.fileForest, miny, maxy);
  } else {
    drawFiles(window, explorer.files, miny, maxy);
  }

  for (int i = 0; i < 4; i++) {
    drawButton(window, explorer.button[i]);
  }

  drawInput(window, explorer.input);

  for (int i = 0; i < 2; i++) {
    drawTextBox(window, explorer.textbox[i]);
  }

  drawButton(window, explorer.button[4]);

  drawScrollbar(window, explorer.scrollbar);
}

void closeExplorer(Explorer &explorer) {
  explorer.files.free();
  free(explorer.fileForest);
}
