#include "explorer.h"

void updateFilesY(list &files, int y) {
  node *p = files.head;
  int fileY = y;
  int offsetY = p->data.background.getGlobalBounds().height / 2 -
                p->data.date.getGlobalBounds().height / 2 +
                p->data.date.getPosition().y -
                p->data.date.getGlobalBounds().top;

  if (y)

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

void drawFiles(RenderWindow &window, list &files) {
  node *p = files.head;

  while (p) {
    drawFile(window, p->data);

    p = p->next;
  }
}

Explorer createExplorer(string path, Font &font, int charSize, int x, int y,
                        int width, int height, ColorTheme theme) {
  Explorer explorer;

  explorer.heightFile = 32; // used for files and file sorting buttons
  explorer.heightComp = 40; // used for anything else

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

  sortFiletree(explorer.files, explorer.sortedBy, explorer.order);

  updateFilesY(explorer.files, y + 2 * explorer.heightComp +
                                   explorer.heightFile + explorer.scrollOffset);

  node *head = explorer.files.head; // head of the file list

  int nameX = x + 1, extX = nameX + head->data.filenameColumn,
      sizeX = extX + head->data.extColumn,
      dateX =
          sizeX +
          head->data.sizeColumn; // used for setting the x of the file columns
  int btnY = y + 2 * explorer.heightComp +
             1; // used for setting the y of the file columns

  // set the drive size text box
  explorer.textbox[0] =
      createTextBox("Size of drive", font, charSize, x + 1, y + 1, width - 2,
                    explorer.heightComp - 2, theme.textMediumContrast,
                    theme.bgLowContrast, theme.border, 1);

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
      "Current folder", font, charSize, x + 1,
      y + height - explorer.heightComp + 1, width - 2, explorer.heightComp - 2,
      theme.textMediumContrast, theme.bgLowContrast, theme.border, 1);

  return explorer;
}

void updateExplorerState(Explorer &explorer, Event event, MouseEventType type,
                         Explorer *&activeExplorer, FloatRect &clickBounds,
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
    updateButtonState(explorer.button[i], event, type, clickBounds);

    if (explorer.button[i].state == B_CLICKED || explorer.button[i].state == B_DCLICKED) {
      // remove the sort indicator from the last button
      explorer.button[explorer.sortedBy].fullText.erase(
          explorer.button[explorer.sortedBy].fullText.size() - 2);
      updateText(
          explorer.button[explorer.sortedBy].text,
          explorer.button[explorer.sortedBy].fullText,
          explorer.button[explorer.sortedBy].background.getGlobalBounds());

      // update sortedBy and order indicators
      if (explorer.sortedBy == sortBy(i)) {
        explorer.order = explorer.order == ASC ? DESC : ASC;
      } else {
        explorer.order = ASC;
      }
      explorer.sortedBy = sortBy(i);

      // append the sort indicator to the new sort button
      explorer.button[explorer.sortedBy].fullText.append(
          explorer.order == ASC ? " /" : " \\");
      updateText(explorer.button[i].text, explorer.button[i].fullText,
                 explorer.button[i].background.getGlobalBounds());

      // sort and update the y of the files
      sortFiletree(explorer.files, explorer.sortedBy, explorer.order);
      updateFilesY(explorer.files,
                   explorer.background.getPosition().y + explorer.heightFile +
                       2 * explorer.heightComp + explorer.scrollOffset);
    }
  }

  FloatRect filelistBounds = explorer.background.getGlobalBounds();
  filelistBounds.top += 2 * explorer.heightComp + explorer.heightFile;
  filelistBounds.height -= (3 * explorer.heightComp + explorer.heightFile);

  if (isHovered(filelistBounds, event.mouseButton.x, event.mouseButton.y)) {
    // update the state of the files
    node *p = explorer.files.head;
    node *start = nullptr, *end = nullptr;

    if (type == DCLICK && explorer.activeFile[0] &&
        isHovered(explorer.activeFile[0]->background.getGlobalBounds(),
                  event.mouseButton.x, event.mouseButton.y)) {
      openFolder(explorer.path, explorer.activeFile[0]->data.filename);

      // use the head of the old list to extract its props
      File file = explorer.files.head->data;

      // update the input of the explorer
      explorer.input.value = explorer.path;
      explorer.input.displayText.setString(explorer.path);
      explorer.input.cursorLocation = explorer.path.size();
      explorer.input.displayLength = explorer.path.size();

      // reset explorer related props
      explorer.textbox[1].fullText = getCurrentFolder(explorer.path);
      explorer.textbox[1].fullText.insert(0, "> ");
      explorer.textbox[1].fullText.append(" <");
      updateText(explorer.textbox[1].text, explorer.textbox[1].fullText,
                 explorer.textbox[1].background.getGlobalBounds());

      // delete old files list
      free(explorer.files);
      init(explorer.files);

      // get the new files from the new path
      getFilesFromPath(
          explorer.files, explorer.path, font, file.filename.getCharacterSize(),
          file.background.getPosition().x, file.background.getPosition().y,
          file.background.getGlobalBounds().width, explorer.heightFile,
          theme.fileStateColors);

      updateFilesY(explorer.files,
                   file.background.getPosition().y - explorer.scrollOffset);

      explorer.scrollOffset = 0;
      explorer.activeFile[0] = explorer.activeFile[1] = nullptr;
    } else {
    while (p) {
      updateFileState(p->data, event, type, explorer.activeFile);

      // if first file has been selected, save the node that has it
      if (explorer.activeFile[0] == &p->data) {
        // reset end pointer if start pointer has been changed
        if (end && start) {
          end = nullptr;
        }

        start = p;
      }

      // if second file has been selected, save the node that has it
      if (explorer.activeFile[1] == &p->data) {
        end = p;
      }

      // unmark anything besides first and second selected
      if (explorer.activeFile[0] != &p->data &&
          explorer.activeFile[1] != &p->data) {
        p->data.state = F_INACTIVE;
      }

      p = p->next;
    }

    // select all files between start and end if they exist
    if (start && end) {
      if (start->data.background.getPosition().y >
          end->data.background.getPosition().y) {
        swap(start, end);
        // swap(explorer.activeFile[0], explorer.activeFile[1]);
      }

      while (start != end) {
        start->data.state = F_SELECTED;
        start = start->next;
      }
    }
  }

  // update the state of the input
  updateInputState(explorer.input, event, type, activeInput);
}

void drawExplorer(RenderWindow &window, Explorer explorer) {
  window.draw(explorer.background);

  drawFiles(window, explorer.files);

  for (int i = 0; i < 4; i++) {
    drawButton(window, explorer.button[i]);
  }

  drawInput(window, explorer.input);

  for (int i = 0; i < 2; i++) {
    drawTextBox(window, explorer.textbox[i]);
  }
}
