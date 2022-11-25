#ifndef FILE_TREE_H
#define FILE_TREE_H
#include "dllist.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum sortBy { FILE_NAME, FILE_SIZE, FILE_DATE };

void getFilesFromPath(list &l, char *path);

void sortFiletree(list &l, sortBy criteria);

node *find(list l, char *filename);

#endif