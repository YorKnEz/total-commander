#include "forest.h"

void init(Forest &forest, List<File> l, int width) {
  forest.length = l.length;

  forest.files = new Forest[forest.length];

  Node<File> *p = l.head;
  FloatRect iconBounds = forest.files[0].data.icon.getGlobalBounds();

  for (int i = 0; i < forest.length; i++) {
    forest.files[i].data = p->data;

    // reset the state
    forest.files[i].data.state = F_INACTIVE;

    // expand the filename
    File crtFile = forest.files[i].data;
    forest.files[i].data.filename.setString(crtFile.data.filename);

    string filename = forest.files[i].data.data.filename;

    // shrink the filename according to the new width
    if (forest.files[i].data.filename.getGlobalBounds().width >
        width - 20 - iconBounds.width - 4) {
      filename.append("..");
      forest.files[i].data.filename.setString(filename);
    }

    while (forest.files[i].data.filename.getGlobalBounds().width >
           width - 20 - iconBounds.width - 4) {
      filename.erase(filename.size() - 3, 1);
      forest.files[i].data.filename.setString(filename);
    }

    // set the new file width
    forest.files[i].data.background.setSize(
        Vector2f(width, crtFile.background.getGlobalBounds().height));

    p = p->next;
  }
}

void printForest(Forest forest, int indent) {
  for (int i = 0; i < forest.length; i++) {
    for (int i = 0; i < indent; i++) {
      cout << "  ";
    }

    if (indent + 1 > 0) {
      cout << (i == forest.length - 1 ? "  " : "  ");
    }

    cout << forest.files[i].data.data.filename << " | ";
    cout << forest.files[i].data.data.ext << " | ";
    cout << forest.files[i].data.data.size << " | ";
    cout << forest.files[i].data.data.date << "\n";

    printForest(forest.files[i], indent + 1);
  }
}

void free(Forest &forest) {
  if (forest.length > 0) {
    for (int i = 0; i < forest.length; i++) {
      free(forest.files[i]);
    }

    delete[] forest.files;
  }
}

int getFileForestHeight(Forest forest, int fileHeight) {
  // get the height of the current forest files
  int height = (fileHeight + 1) * forest.length;

  for (int i = 0; i < forest.length; i++) {
    // add the height of the non hidden forests
    if (!forest.files[i].hidden) {
      height += getFileForestHeight(forest.files[i], fileHeight);
    }
  }

  return height;
}

void updateFileForestXY(Forest &forest, int x, int &y) {
  for (int i = 0; i < forest.length; i++) {
    FloatRect backgroundBounds =
        forest.files[i].data.background.getGlobalBounds();
    FloatRect iconBounds = forest.files[i].data.icon.getGlobalBounds();
    int charSize = forest.files[i].data.filename.getCharacterSize();

    int offsetYText = (backgroundBounds.height - charSize) / 2;
    int offsetYIcon = (backgroundBounds.height - iconBounds.height) / 2;

    forest.files[i].data.background.setPosition(x, y);
    forest.files[i].data.icon.setPosition(x + 10, y + offsetYIcon);
    forest.files[i].data.filename.setPosition(x + 10 + iconBounds.width + 4,
                                              y + offsetYText);

    y += forest.files[i].data.background.getGlobalBounds().height + 1;

    if (!forest.files[i].hidden) {
      updateFileForestXY(forest.files[i], x + 20, y);
    }
  }
}

void updateFileForestState(Forest forest, string &activeForestPath,
                           Forest *&activeForest, string path, Event event,
                           MouseEventType type, Vector2i &oldClick) {
  Keyboard kbd; // used for checking combos

  for (int i = 0; i < forest.length; i++) {
    switch (type) {
    case DCLICK:
    case CLICK:
      if (isHovered(forest.files[i].data.background.getGlobalBounds(),
                    event.mouseButton.x, event.mouseButton.y)) {
        if (activeForest) {
          activeForest->data.state = F_INACTIVE;
          activeForest = nullptr;
        }

        activeForestPath = path;
        activeForest = &forest.files[i];
        activeForest->data.state = F_SELECTED;
      }
      break;
    }

    if (!forest.files[i].hidden) {
      updateFileForestState(forest.files[i], activeForestPath, activeForest,
                            path + SEP + forest.files[i].data.data.filename,
                            event, type, oldClick);
    }
  }
}

void updateFileForestTheme(Forest &forest,
                           FileStateColors fileStateColors[F_MAX_STATES]) {

  for (int i = 0; i < forest.length; i++) {
    updateFileTheme(forest.files[i].data, fileStateColors);
    updateFileForestTheme(forest.files[i], fileStateColors);
  }
}

void drawFileForest(RenderWindow &window, Forest forest, int miny, int maxy) {
  for (int i = 0; i < forest.length; i++) {
    // get bounds of the current file to check if it's outside of the screen
    FloatRect currentFileBounds =
        forest.files[i].data.background.getGlobalBounds();

    // draw the current file only if it's on the screen
    if (currentFileBounds.top + currentFileBounds.height >= miny &&
        currentFileBounds.top <= maxy) {
      bool displayFilenameOnly = true;
      drawFile(window, forest.files[i].data, displayFilenameOnly);
    }

    // draw the files nested underneath it if they're not hidden
    if (!forest.files[i].hidden) {
      drawFileForest(window, forest.files[i], miny, maxy);
    }
  }
}
