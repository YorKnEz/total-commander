#include "file.h"

File createFile(Filedata data, Font &font, int charSize, int x, int y,
                int width, int height,
                FileStateColors stateColors[F_MAX_STATES], FileIcons &fileIcons,
                int borderThickness) {
  File file;
  file.state = F_INACTIVE; // set the state of the button
  // copy the state colors
  for (int i = 0; i < F_MAX_STATES; i++) {
    file.stateColors[i] = stateColors[i];
  }

  // set the file icon
  if (data.size == "<DIR>") {
    file.icon.setTexture(fileIcons.folder);
  } else if (data.ext == "png" || data.ext == "jpg" || data.ext == "jpeg" ||
             data.ext == "svg" || data.ext == "gif") {
    file.icon.setTexture(fileIcons.image);
  } else if (data.ext == "pdf") {
    file.icon.setTexture(fileIcons.pdf);
  } else if (data.ext == "txt") {
    file.icon.setTexture(fileIcons.txt);
  } else {
    file.icon.setTexture(fileIcons.file);
  }

  FloatRect iconBounds = file.icon.getGlobalBounds();

  // initialize background
  file.background =
      createRect(x, y, width, height, file.stateColors[file.state].background,
                 file.stateColors[file.state].border, borderThickness);

  // initialize data
  file.data = data;

  // initialize column sizes
  Text date("dd/mm/yyyy hh:mm xx", font, charSize);

  file.dateColumn = date.getGlobalBounds().width + 20;
  file.extColumn = (width - file.dateColumn) / 4;
  file.filenameColumn = (width - file.dateColumn) / 2;
  file.sizeColumn = (width - file.dateColumn) / 4;

  // initialize text fields
  int nameX = x + 10, extX = nameX + file.filenameColumn,
      sizeX = extX + file.extColumn, dateX = sizeX + file.sizeColumn;

  file.filename = createText(data.filename, font, charSize, nameX, y,
                             file.filenameColumn - 20 - 4 - iconBounds.width,
                             file.stateColors[file.state].textHighContrast);
  file.ext = createText(data.ext, font, charSize, extX, y, file.extColumn - 20,
                        file.stateColors[file.state].textLowContrast);
  file.size =
      createText(data.size, font, charSize, sizeX, y, file.sizeColumn - 20,
                 file.stateColors[file.state].textLowContrast);
  file.date =
      createText(data.date, font, charSize, dateX, y, file.dateColumn - 20,
                 file.stateColors[file.state].textLowContrast);

  FloatRect backgroundBounds = file.background.getGlobalBounds();
  FloatRect textBounds = file.filename.getLocalBounds();

  int offsetYText = (backgroundBounds.height - charSize) / 2;
  int offsetYIcon = (backgroundBounds.height - iconBounds.height) / 2;

  file.filename.setPosition(x + iconBounds.width + 14, y + offsetYText);

  file.icon.setPosition(x + 10, y + offsetYIcon);

  return file;
}

void updateFileTheme(File &file, FileStateColors stateColors[F_MAX_STATES]) {
  for (int i = 0; i < F_MAX_STATES; i++) {
    file.stateColors[i] = stateColors[i];
  }

  file.background.setFillColor(stateColors[file.state].background);
  file.background.setOutlineColor(stateColors[file.state].border);

  file.filename.setFillColor(stateColors[file.state].textHighContrast);
  file.ext.setFillColor(stateColors[file.state].textLowContrast);
  file.size.setFillColor(stateColors[file.state].textLowContrast);
  file.date.setFillColor(stateColors[file.state].textLowContrast);
}

void drawFile(RenderWindow &window, File file, bool filenameOnly) {
  // update the color of the file depending on it's state
  file.filename.setFillColor(file.stateColors[file.state].textHighContrast);

  if (!filenameOnly) {
    file.ext.setFillColor(file.stateColors[file.state].textLowContrast);
    file.size.setFillColor(file.stateColors[file.state].textLowContrast);
    file.date.setFillColor(file.stateColors[file.state].textLowContrast);
  }

  file.background.setFillColor(file.stateColors[file.state].background);
  file.background.setOutlineColor(file.stateColors[file.state].border);

  window.draw(file.background);
  window.draw(file.icon);
  drawText(window, file.filename);

  if (!filenameOnly) {
    drawText(window, file.ext);
    drawText(window, file.size);
    drawText(window, file.date);
  }
}
