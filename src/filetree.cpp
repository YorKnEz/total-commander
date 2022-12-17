#include "filetree.h"

// sort nodes by extension
bool byExt(node *a, node *b, sortOrder order) {
  // try to sort by extension
  return order *
             (toLower(a->data.data.ext).compare(toLower(b->data.data.ext))) <=
         0;
}

// sort nodes by name
bool byName(node *a, node *b, sortOrder order) {
  // try to sort by filename
  int comp =
      order *
      toLower(a->data.data.filename).compare(toLower(b->data.data.filename));

  if (comp) {
    return comp <= 0;
  }

  // try to sort by extenions
  return byExt(a, b, order);
}

// sort nodes by size
bool bySize(node *a, node *b, sortOrder order) {
  // try to sort by length of sizes
  if (a->data.data.size.size() > b->data.data.size.size()) {
    return order != ASC;
  } else if (a->data.data.size.size() < b->data.data.size.size()) {
    return order == ASC;
  }

  // try to sort by comparing the numbers
  int comp = order * (a->data.data.size.compare(b->data.data.size));

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
    substrA = a->data.data.date.substr(substrPositions[i].start,
                                       substrPositions[i].len);
    substrB = b->data.data.date.substr(substrPositions[i].start,
                                       substrPositions[i].len);

    comp = order * substrA.compare(substrB);

    if (comp) {
      return comp <= 0;
    }
  }

  return byName(a, b, order);
}

// returns a string with the default path depending on the OS
string getDefaultPath() {
  string path;
#ifdef _WIN32
  path = "x:\\";
  for (int i = 0; i < 26; i++) {
    path[0] = char(i + 'A');
    if (fs::exists(path))
      break;
  }
#endif
#ifdef __linux__
  path = "home";
#endif
  return path;
}

// generates a list of files containing data about the path's content
void getFilesFromPath(list &l, string path, Font &font, int charSize, int x,
                      int y, int width, int height,
                      FileStateColors stateColors[B_MAX_STATES]) {
  // lastDir is used in order to separate files from directories in the list.
  // The directories are inserted after the last directory or at the beginning
  // of the list if there are none, and the files are always added at the end.
  int lastDir = 0;

  Filedata filedata;

  // adds the special ".." folder to the list
  const auto fileTime = fs::last_write_time("..");
  const auto systemTime = chrono::file_clock::to_sys(fileTime);
  const auto time = chrono::system_clock::to_time_t(systemTime) + 7200;
  string date = asctime(gmtime(&time));

  // generates the filedata for the special folder
  filedata.filename = "..";
  filedata.size = "<DIR>";
  filedata.date = formatDate(date);
  filedata.ext = "";

  File element =
      createFile(filedata, font, charSize, x, y, width, height, stateColors);

  add(l, element, lastDir);
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

    element =
        createFile(filedata, font, charSize, x, y, width, height, stateColors);

    if (!size.compare("<DIR>")) {
      add(l, element, lastDir);

      lastDir++;
    } else {
      add(l, element, l.length);
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
    if (!p->data.data.filename.compare(filename)) {
      return p;
    }

    p = p->next;
  }

  return NULL;
}

bool checkIfValid(string path) {
  // the separator is different between linux and windows
  string separator = path.find_last_of("\\") != path.npos ? "\\" : "/";
  if (!fs::exists(path))
    return false;
  string folderName = path;
  folderName = folderName.erase(0, folderName.find_last_of(separator));
  if (folderName.find("\\/\n#<>$+%!'&*`|{}?\"=:@") != folderName.npos)
    return false;
  return true;
}

// goes into the next folder
void openFolder(string &path, string name) {
  // the separator is different between linux and windows
  string separator;
  if (path.back() != '\\' and path.back() != '/')
    separator = path.find_last_of("\\") != path.npos ? "\\" : "/";
  else
    separator.clear();
  if (name.compare("..") == 0) {
    if (path.back() == '\\' or path.back() == '/')
      return;
    else {
      path = path.erase(path.find_last_of(separator), path.npos);
      if (path.back() == ':')
        path.append(separator);
      return;
    }
  }
  if (!checkIfValid(path) or !checkIfValid(path + separator + name)) {
    return;
  }
  if (!fs::is_directory(path + separator + name))
    return;
  path = path + separator + name;
}

// creates a new folder
void createFolder(string path, string name) {
  // the separator is different between linux and windows
  string separator = path.find_last_of("\\") != path.npos ? "\\" : "/";

  // checks if the folder already exists, case in which we add "(counter)" at
  // the end of the name
  string folderName = name;
  int counter = 0;
  while (fs::exists(path + separator + name)) {
    name = folderName + " (" + int2str(++counter) + ")";
  }

  // creates a folder with the specified name
  fs::create_directory(path + separator + name);
}

// copies a file from a path to another path
void copyFile(string fromPath, string toPath) {
  // initialize files and buffer
  FILE *fromPtr, *toPtr;
  char buffer[1024];
  size_t bytes;

  // checks if the file to be copied exists
  fromPtr = fopen(fromPath.c_str(), "rb");
  if (!fromPtr) {
    perror("File not found.");
    return;
  }

  // saves the extension of the file
  string extension = fromPath;
  extension = extension.erase(0, extension.find_last_of("."));

  // checks if a file already exists with the same name and extension, in which
  // case we add "(counter)" at the end
  string doesExist = toPath;
  int counter = 0;
  while (fopen((toPath + extension).c_str(), "rb") != NULL) {
    toPath = doesExist + " (" + int2str(++counter) + ")";
  }

  // opens the file from "toPath" in write binary mode
  toPath = toPath + extension;
  toPtr = fopen(toPath.c_str(), "wb");

  // copies the file from "fromPath" byte by byte to the file from "toPath" and
  // then closes the files
  while (0 < (bytes = fread(buffer, 1, sizeof(buffer), fromPtr))) {
    fwrite(buffer, 1, bytes, toPtr);
  }
  fclose(fromPtr);
  fclose(toPtr);
}

// copies a folder and its components from a path to another path
void copyFolder(string fromPath, string toPath) {
  // the separator is different between linux and windows
  string separator = fromPath.find_last_of("\\") != fromPath.npos ? "\\" : "/";

  // creates the folder in the path stored in toPath, but checks if it already
  // exists, case in which it generates with the format "name (x)", where x is
  // counting how many copies there are already with the same format
  string name = toPath;
  name = name.erase(0, name.find_last_of(separator) + 1);
  toPath = toPath.erase(toPath.find_last_of(separator), toPath.npos);
  string folderName = name;
  int counter = 0;
  while (fs::exists(toPath + separator + name)) {
    name = folderName + " (" + int2str(++counter) + ")";
  }
  createFolder(toPath, name);
  toPath += separator + name;

  // iterates in the current directory and if the entry is a directory we recall
  // the function recursively and when the entry is a file we copy it
  for (const auto &entry : fs::directory_iterator(fromPath)) {
    fs::path directoryPath = entry.path().filename();
    string filename = directoryPath.generic_string();
    if (entry.is_directory()) {
      copyFolder(fromPath + separator + filename,
                 toPath + separator + filename);
    } else
      copyFile(fromPath + separator + filename, toPath + separator + filename);
  }
}

// deletes a file from a specified path
void deleteFile(string path) { fs::remove(path); }

// deletes a folder from a specified path and its components
void deleteFolder(string path) {
  // the separator is different between linux and windows
  string separator = path.find_last_of("\\") != path.npos ? "\\" : "/";
  for (const auto &entry : fs::directory_iterator(path)) {
    fs::path directoryPath = entry.path().filename();
    string filename = directoryPath.generic_string();
    if (entry.is_directory())
      deleteFolder(path + separator + filename);
    deleteFile(path + separator + filename);
  }
  deleteFile(path);
}

// moves a file from a path to another path
void moveFile(string fromPath, string toPath) {
  copyFile(fromPath, toPath);
  deleteFile(fromPath);
}

// moves a folder from a path to another path
void moveFolder(string fromPath, string toPath) {
  copyFolder(fromPath, toPath);
  deleteFolder(fromPath);
}

// renames a file
void editFileName(string path, string newName) {
  // the separator is different between linux and windows
  string separator = path.find_last_of("\\") != path.npos ? "\\" : "/";

  string folder = path;
  folder = folder.erase(folder.find_last_of(separator) + 1);
  copyFile(path, folder + newName);
  deleteFile(path);
}

// renames a folder
void editFolderName(string path, string newName) {
  // the separator is different between linux and windows
  string separator = path.find_last_of("\\") != path.npos ? "\\" : "/";

  string folder = path;
  folder = folder.erase(folder.find_last_of(separator) + 1);
  copyFolder(path, folder + newName);
  deleteFolder(path);
}
