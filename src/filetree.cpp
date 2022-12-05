#include "filetree.h"

// sort nodes by extension
bool byExt(node *a, node *b, sortOrder order) {
  // try to sort by extension
  return order * (toLower(a->data.ext).compare(toLower(b->data.ext))) <= 0;
}

// sort nodes by name
bool byName(node *a, node *b, sortOrder order) {
  // try to sort by filename
  int comp = order * toLower(a->data.filename).compare(toLower(b->data.filename));

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
  Filedata currentData;

  // lastDir is used in order to separate files from directories in the list.
  // The directories are inserted after the last directory or at the beginning
  // of the list if there are none, and the files are always added at the end.
  int lastDir = 0;

  // adds the special ".." folder to the list
  const auto fileTime = fs::last_write_time("..");
  const auto systemTime = chrono::file_clock::to_sys(fileTime);
  const auto time = chrono::system_clock::to_time_t(systemTime) + 7200;
  string date = asctime(gmtime(&time));

  Filedata filedata;

  // generates the filedata for the special folder
  filedata.filename = "..";
  filedata.size = "<DIR>";
  filedata.date = formatDate(date);
  filedata.ext = "";

  add(l, filedata, lastDir);
  lastDir++;

  // goes through the content of the current path
  for (const auto &entry : fs::directory_iterator(path)) {
    // converts filename path to string
    fs::path directoryPath = entry.path().filename();
    string filename = directoryPath.generic_string();

    // gets the date at which the current file was last modified
    const auto fileTime = fs::last_write_time(entry.path());
    // converts the result to system time
    const auto systemTime = chrono::file_clock::to_sys(fileTime);
    // converts the new result to time_t (seconds that have passed since epoch
    // time - 1 January 1970 00:00:00) / to UNIX timestamp
    const auto time = chrono::system_clock::to_time_t(systemTime) + 7200;

    // checks whether the current entry is a file or a directory and sets the
    // size accordingly (directory size = "<DIR>", file size is an integer
    // value)
    string size = is_regular_file(entry.path())
                      ? int2str(file_size(entry.path()))
                      : "<DIR>";
    if (size == "") {
      size = "0";
    }

    // generates a converted string from timestamp to GMT
    string date = asctime(gmtime(&time));
    Filedata filedata;

    // generates the filedata with the previously obtained strings and adds them
    // to the list
    filedata.filename = filename;
    filedata.size = size;
    filedata.date = formatDate(date);
    int lastDotPos = filedata.filename.find_last_of('.');

    if (lastDotPos != string::npos && lastDotPos != 0 &&
        filedata.size.compare("<DIR>")) {
      filedata.ext = filedata.filename.substr(lastDotPos + 1);
      filedata.filename.erase(filedata.filename.find_last_of('.'));
    } else
      filedata.ext = "";

    if (!size.compare("<DIR>")) {
      add(l, filedata, lastDir);
      lastDir++;
    } else {
      add(l, filedata, l.length);
    }
  }
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
