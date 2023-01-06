#ifndef FOREST_H
#define FOREST_H

#include "file.h"
#include "filetree.h"
#include <iostream>
#include <string>

using namespace std;

struct Forest {
  bool hidden = false; // whether it should be drawn or not
  File data;
  unsigned int length = 0;
  Forest *files;
};

// init the files and length props of the Forest
void init(Forest &forest, List<File> l, int width);

void printForest(Forest forest, int indent);

void free(Forest &forest);

// graphics related functions

// returns the height of the forest in pixels
int getFileForestHeight(Forest forest, int fileHeight);

// updates the screen position of each file recursively
void updateFileForestXY(Forest &forest, int x, int &y);

// checks if any of the files has been clicked and stores it in
// `activeForestPath` and `activeForest`
void updateFileForestState(Forest forest, string &activeForestPath,
                           Forest *&activeForest, string path, Event event,
                           MouseEventType type, Vector2i &oldClick);

void updateFileForestTheme(Forest &forest,
                           FileStateColors fileStateColors[F_MAX_STATES]);

// draws the files on the screen recursively
void drawFileForest(RenderWindow &window, Forest forest, int miny, int maxy);

#endif
