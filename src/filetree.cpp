#include "filetree.h"

bool byName(node *a, node *b) {
  return strcmp(a->data.filename, b->data.filename) <= 0;
}

bool bySize(node *a, node *b) { return a->data.size < b->data.size; }

bool byDate(node *a, node *b) {
  // implement date comparison
}

void getFilesFromPath(list &l, char *path) {
  // we call system("dir path >> buffer.txt");
  char bufferFileName[] = "buffer.txt";
  char command[100];

  strcpy(command, "dir ");
  strcat(command, path);
  strcat(command, " > ");
  strcat(command, bufferFileName);

  system(command);

  FILE *bufferFile = fopen(bufferFileName, "r");

  if (!bufferFile) {
    perror("File not found");
    return;
  }

  char currentFileData[1000];

  // first 6 lines are unnecessary
  for (int i = 0; i < 6; i++) {
    fgets(currentFileData, 1000, bufferFile);
  }

  Filedata currentData;
  int lastDir = 0;
  // read the filedataStrings from the file while the first character is not a
  // space if it is a space, it means we got to the last two lines
  while (fgets(currentFileData, 1000, bufferFile) &&
         currentFileData[0] != ' ') {
    if (currentFileData[0] != ' ') {
      currentData = parseFileDataString(currentFileData);

      if (currentData.size == -1) {
        add(l, currentData, lastDir);
        lastDir++;
      } else {
        add(l, currentData, l.length);
      }
    }
  }
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
    printf("Invalid sort option\n");
    break;
  }
}

node *find(list l, char *filename) {
  node *p = l.head;

  while (p) {
    if (!strcmp(p->data.filename, filename)) {
      return p;
    }

    p = p->next;
  }

  return NULL;
}
