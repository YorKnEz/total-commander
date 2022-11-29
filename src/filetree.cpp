#include "filetree.h"

bool byName(node *a, node *b) {
  return a->data.filename.compare(b->data.filename) <= 0;
}

bool bySize(node *a, node *b) { return a->data.size < b->data.size; }

bool byDate(node *a, node *b) {
  return false;
  // implement date comparison
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

void sortFiletree(list &l, sortBy criteria) {
  switch (criteria) {
  case FILE_NAME:
    sort(l, byName);
    break;
  case FILE_SIZE:
    sort(l, bySize);
    break;
  case FILE_DATE:
    sort(l, byDate);
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