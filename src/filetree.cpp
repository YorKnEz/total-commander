#include "filetree.h"

// sort nodes by extension
bool byExt(node *a, node *b, sortOrder order) {
  // try to sort by extension
  return order * (a->data.ext.compare(b->data.ext)) <= 0;
}

// sort nodes by name
bool byName(node *a, node *b, sortOrder order) {
  // try to sort by filename
  int comp = order * a->data.filename.compare(b->data.filename);

  if (comp) {
    return comp <= 0;
  }

  // try to sort by extenions
  return byExt(a, b, order);
}

// sort nodes by size
bool bySize(node *a, node *b, sortOrder order) {
  // try to sort by length of sizes
  if (a->data.size.size() > b->data.size.size()) {
    return order != ASC;
  } else if (a->data.size.size() < b->data.size.size()) {
    return order == ASC;
  }

  // try to sort by comparing the numbers
  int comp = order * (a->data.size.compare(b->data.size));

  if (comp) {
    return comp <= 0;
  }

  // try to sort by filename
  return byName(a, b, order);
}

// sort nodes by date
bool byDate(node *a, node *b, sortOrder order) {
  // date is of format "dd/mm/yyyy hh:mm xx", where xx is AM or PM
  // we use the following struct to store the start and length of a token from
  // the string a token can be: dd, mm, yyyy etc.
  struct {
    int start,
        len; // start position and length of each of the tokens mentioned above
  } substrPositions[] = {{6, 4}, {3, 2}, {0, 2}, {17, 2}, {11, 2}, {14, 2}};

  int comp;
  string substrA, substrB; // for storing the year, month etc. when comparing

  for (int i = 0; i < 6; i++) {
    substrA =
        a->data.date.substr(substrPositions[i].start, substrPositions[i].len);
    substrB =
        b->data.date.substr(substrPositions[i].start, substrPositions[i].len);

    comp = order * substrA.compare(substrB);

    if (comp) {
      return comp <= 0;
    }
  }

  return byName(a, b, order);
}

void getFilesFromPath(list &l, string path) {
  // we call system("dir path >> buffer.txt");
  string bufferFileName = "buffer.txt";
  string command = "dir \"" + path + "\" > " + bufferFileName;

  system(command.c_str());

  FILE *bufferFile = freopen(bufferFileName.c_str(), "r", stdin);

  if (!bufferFile) {
    cout << "File not found";
    return;
  }

  string currentBufferLine;

  // first 6 lines are unnecessary
  for (int i = 0; i < 6; i++) {
    getline(cin, currentBufferLine);
  }

  Filedata currentData;
  int lastDir = 0;
  // read the filedataStrings from the file while the first character is not a
  // space if it is a space, it means we got to the last two lines
  while ((getline(cin, currentBufferLine)) && currentBufferLine[0] != ' ') {
    if (currentBufferLine[0] != ' ') {
      currentData = parseFileDataString(currentBufferLine);

      if (!currentData.size.compare("<DIR>")) {
        add(l, currentData, lastDir);
        lastDir++;
      } else {
        add(l, currentData, l.length);
      }
    }
  }

  fclose(stdin);
  remove(bufferFileName.c_str());
}

// sorting keeps the directories before any files and sorts them separately
// it also assumes that directories have been put before any files by default
void sortFiletree(list &l, sortBy criteria, sortOrder order) {
  switch (criteria) {
  case FILE_NAME:
    sort(l, order, byName);
    break;
  case FILE_EXT:
    sort(l, order, byExt);
    break;
  case FILE_SIZE:
    sort(l, order, bySize);
    break;
  case FILE_DATE:
    sort(l, order, byDate);
    break;
  default:
    cout << "Invalid sort option\n";
    break;
  }
}

node *find(list l, string filename) {
  node *p = l.head;

  while (p) {
    if (!p->data.filename.compare(filename)) {
      return p;
    }

    p = p->next;
  }

  return NULL;
}

void copyFile(string fromPath, string toPath) {
  string command = "copy \"" + fromPath + "\" \"" + toPath + "\"";
  system(command.c_str());
}

void moveFile(string fromPath, string toPath) {
  string command = "copy \"" + fromPath + "\" \"" + toPath + "\"";
  system(command.c_str());
  command = "del \"" + fromPath + "\"";
  system(command.c_str());
};

void deleteFile(string path) {

  string command = "del \"" + path + "\"";
  system(command.c_str());
}

void deleteFolder(string path) {

  string command = "erase \"" + path + "\"";
  system(command.c_str());
}

void editFileName(string path, string newName) {
  string command = "ren \"" + path + "\" \"" + newName + "\"";
  system(command.c_str());
}

void createFolder(string path, string name) {
  string command = "md \"" + path + "\\" + name + "\"";
  system(command.c_str());
}

void openFolder(string &path, string name) {
  path = "\"" + path + "\\" + name + "\"";
}