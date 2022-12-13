#include "explorer.h"

void updateFilesY(list &files, int y) {
  node *p = files.head;
  int fileY = y;
  int offsetY = p->data.background.getGlobalBounds().top +
                p->data.background.getGlobalBounds().height / 2 -
                p->data.date.getGlobalBounds().top -
                p->data.date.getGlobalBounds().height / 2;

  while (p) {
    p->data.background.setPosition(p->data.background.getPosition().x, fileY);
    p->data.filename.setPosition(p->data.filename.getPosition().x,
                                 fileY + offsetY);
    p->data.ext.setPosition(p->data.ext.getPosition().x, fileY + offsetY);
    p->data.size.setPosition(p->data.size.getPosition().x, fileY + offsetY);
    p->data.date.setPosition(p->data.date.getPosition().x, fileY + offsetY);

    fileY += p->data.background.getGlobalBounds().height + 4;

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

  int height1 = 32; // used for files and file sorting buttons
  int height2 = 40; // used for anything else

  explorer.path = path; // set path
  explorer.scrollOffset = 0;

  // initialize the background
  explorer.background.setSize(Vector2f(width, height - 3 * height2));
  explorer.background.setFillColor(theme.bgBody);
  explorer.background.setPosition(x, y + 2 * height2);

  // initialize the files list
  init(explorer.files);

  getFilesFromPath(explorer.files, path, font, charSize, x,
                   y + 2 * height2 + height1, width - 20, height1,
                   theme.textHighContrast, theme.textLowContrast, theme.border,
                   theme.border);

  updateFilesY(explorer.files,
               y + 2 * height2 + height1 + explorer.scrollOffset);

  node *head = explorer.files.head; // head of the file list

  int nameX = x + 1, extX = nameX + head->data.filenameColumn,
      sizeX = extX + head->data.extColumn,
      dateX =
          sizeX +
          head->data.sizeColumn;  // used for setting the x of the file columns
  int btnY = y + 2 * height2 + 1; // used for setting the y of the file columns

  // set the drive size text box
  explorer.textbox[0] = createTextBox(
      "Size of drive", font, charSize, x + 1, y + 1, width - 2, height2 - 2,
      theme.textMediumContrast, theme.bgLowContrast, theme.border, 1);

  // set the input
  explorer.input = createInput("Enter path here", path, font, charSize, x + 1,
                               y + height2 + 1, width - 2, height2 - 2,
                               theme.inputStateColors, 1);

  // set the sorting buttons
  explorer.button[0] = createButton("Name", font, charSize, nameX, btnY,
                                    head->data.filenameColumn - 2, height1 - 2,
                                    theme.buttonStateColors, 1);
  explorer.button[1] =
      createButton("Ext", font, charSize, extX, btnY, head->data.extColumn - 2,
                   height1 - 2, theme.buttonStateColors, 1);
  explorer.button[2] = createButton("Size", font, charSize, sizeX, btnY,
                                    head->data.sizeColumn - 2, height1 - 2,
                                    theme.buttonStateColors, 1);
  explorer.button[3] = createButton("Date", font, charSize, dateX, btnY,
                                    head->data.dateColumn - 1, height1 - 2,
                                    theme.buttonStateColors, 1);

  // set the current folder text box
  explorer.textbox[1] = createTextBox("Current folder", font, charSize, x + 1,
                                      y + height - height2 + 1, width - 2,
                                      height2 - 2, theme.textMediumContrast,
                                      theme.bgLowContrast, theme.border, 1);

  return explorer;
}

void updateExplorerState(Explorer &explorer, Event event, MouseEventType type,
                         Explorer *&activeExplorer, FloatRect &clickBounds,
                         Input *&activeInput) {
  switch (type) {
  case CLICK:
    // if the user clicks inside the explorer, move the focus to the explorer
    if (isHovered(explorer.background.getGlobalBounds(), event.mouseButton.x,
                  event.mouseButton.y)) {
      if (activeExplorer) {
        activeExplorer->state = E_INACTIVE; // set old explorer as inactive

        // remove the focused explorer indicator
        activeExplorer->textbox[1].fullText.erase(0, 2);
        activeExplorer->textbox[1].fullText.erase(
            activeExplorer->textbox[1].fullText.size() - 2, 2);

        activeExplorer->textbox[1].text.setString(
            activeExplorer->textbox[1].fullText);
      }

      activeExplorer = &explorer;
      activeExplorer->state = E_ACTIVE; // set current explorer as active

      // add the focused explorer indicator
      activeExplorer->textbox[1].fullText.insert(0, "> ");
      activeExplorer->textbox[1].fullText.append(" <");

      activeExplorer->textbox[1].text.setString(
          activeExplorer->textbox[1].fullText);
    }
    break;
  }

  for (int i = 0; i < 4; i++) {
    updateButtonState(explorer.button[i], event, type, clickBounds);

    if (explorer.button[i].state == B_CLICKED) {
      sortFiletree(explorer.files, sortBy(i), ASC);
      updateFilesY(explorer.files, explorer.background.getPosition().y + 32 +
                                       explorer.scrollOffset);
    }
  }

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
