#include "filetree.h"

// sort nodes by extension
bool byExt(Node<File> *a, Node<File> *b, sortOrder order) {
  // try to sort by extension
  return order *
             (toLower(a->data.data.ext).compare(toLower(b->data.data.ext))) <=
         0;
}

// sort nodes by name
bool byName(Node<File> *a, Node<File> *b, sortOrder order) {
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
bool bySize(Node<File> *a, Node<File> *b, sortOrder order) {
  // try to sort by order of size (KB, MB, GB, TB)
  string sizeA = a->data.data.size, sizeB = b->data.data.size;
  string dim = " KMGT";
  if (dim.find(sizeA[sizeA.size() - 2]) > dim.find(sizeB[sizeB.size() - 2])) {
    return order != ASC;
  } else if (dim.find(sizeA[sizeA.size() - 2]) <
             dim.find(sizeB[sizeB.size() - 2])) {
    return order == ASC;
  }

  string intPartA = sizeA.substr(0, sizeA.find("."));
  string intPartB = sizeB.substr(0, sizeB.find("."));

  // try to sort by comparing the sizes of the int numbers
  if (intPartA.size() > intPartB.size()) {
    return order != ASC;
  } else if (intPartA.size() < intPartB.size()) {
    return order == ASC;
  }

  // try to sort by comparing the int numbers
  int comp = order * (intPartA.compare(intPartB));

  if (comp) {
    return comp <= 0;
  }

  // try to sort by comparing the decimal numbers
  if (sizeA[sizeA.find(".") + 1] > sizeB[sizeB.find(".") + 1]) {
    return order != ASC;
  } else if (sizeA[sizeA.find(".") + 1] < sizeB[sizeB.find(".") + 1]) {
    return order == ASC;
  }

  // try to sort by filename
  return byName(a, b, order);
}

// sort nodes by date
bool byDate(Node<File> *a, Node<File> *b, sortOrder order) {
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

void searchFile(List<File> &results, string path, string searchQuery,
                Theme &theme, int x, int y, int width, int height) {
  for (const auto &entry : fs::directory_iterator(path)) {
    try {
      string filename = (entry.path().filename()).generic_string();

      if (toLower(filename).find(toLower(searchQuery)) != string::npos) {
        File result = getFileFromEntry(
            entry, theme, x, y, width,
            max(2 * theme.charSize + 4, int(theme.diagram.getSize().y)) + 8);

        result.data.path = path;

        result.path = createText(
            path, theme.font, theme.charSize,
            result.filename.getGlobalBounds().left,
            result.filename.getGlobalBounds().top,
            result.filenameColumn - 20 - result.icon.getGlobalBounds().width - 4,
            result.stateColors[result.state].textLowContrast);

        results.add(result, results.length);
      }

      string newPath = path + SEP + filename;

      if (fs::is_directory(newPath)) {
        searchFile(results, newPath, searchQuery, theme, x, y, width, height);
      }
    } catch (fs::filesystem_error) {
    }
  }
}

// returns a string with the default path depending on the OS
string getDefaultPath() {
  string path;
#if defined _WIN32
  path = "x:\\";
  for (int i = 0; i < 26; i++) {
    path[0] = char(i + 'A');
    if (isValidPath(path))
      break;
  }
#elif defined __linux__
  path = "/";
#endif
  return path;
}

File getFileFromEntry(const auto entry, Theme &theme, int x, int y, int width,
                      int height) {
  // converts filename path to string
  fs::path directoryPath = entry.path().filename();
  string filename = directoryPath.generic_string();
  string size;

  // gets the date at which the current file was last modified
  const auto fileTime = fs::last_write_time(entry.path());
  // converts the result to system time
  const auto systemTime = chrono::file_clock::to_sys(fileTime);
  // converts the new result to time_t (seconds that have passed since
  // epoch time - 1 January 1970 00:00:00) / to UNIX timestamp
  const auto time = chrono::system_clock::to_time_t(systemTime) + 7200;

  // checks whether the current entry is a file or a directory and sets
  // the size accordingly (directory size = "<DIR>", file size is an
  // integer value)
  if (is_regular_file(entry.path())) {
    size = compressSize(file_size(entry.path()));
  } else
    size = "<DIR>";

  // generates a converted string from timestamp to GMT
  string date = asctime(gmtime(&time));
  Filedata filedata;

  // generates the filedata with the previously obtained strings and adds
  // them to the list
  filedata.filename = filename;
  filedata.size = size;
  filedata.date = formatDate(date);

  int lastDotPos = filedata.filename.find_last_of('.');

  if (lastDotPos != string::npos && lastDotPos != 0 &&
      filedata.size.compare("<DIR>")) {
    filedata.ext = filedata.filename.substr(lastDotPos + 1);
    filedata.filename.erase(filedata.filename.find_last_of('.'));
  } else {
    filedata.ext = "";
  }

  return createFile(filedata, theme.font, theme.charSize, x, y, width, height,
                    theme.colors.fileStateColors, theme.fileIcons);
}

// generates a list of files containing data about the path's content
void getFilesFromPath(List<File> &l, string path, int x, int y, int width,
                      int height, Theme &theme, bool ignoreBackwardsFolder) {
  // lastDir is used in order to separate files from directories in the list.
  // The directories are inserted after the last directory or at the beginning
  // of the list if there are none, and the files are always added at the end.
  int lastDir = 0;

  Filedata filedata;
  File element;

  if (path != getDefaultPath() && !ignoreBackwardsFolder) {
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

    element = createFile(filedata, theme.font, theme.charSize, x, y, width,
                         height, theme.colors.fileStateColors, theme.fileIcons);

    l.add(element, lastDir);
    lastDir++;
  }
  // goes through the content of the current path
  for (const auto &entry : fs::directory_iterator(path)) {
    try {
      element = getFileFromEntry(entry, theme, x, y, width, height);

      if (!element.data.size.compare("<DIR>")) {
        l.add(element, lastDir);

        lastDir++;
      } else {
        l.add(element, l.length);
      }
    } catch (fs::filesystem_error) {
    }
  }
}

// sorting keeps the directories before any files and sorts them separately
// it also assumes that directories have been put before any files by default
void sortFiletree(List<File> &l, sortBy criteria, sortOrder order) {
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

string evalPath(string path) {
  if (path == "" || (path.back() == ':' && path.size() == 2)) {
    path.append(SEP);
    return path;
  }

  if (!isValidPath(path)) {
    return "invalid";
  }

  string initialPath = path;

  // removes duplicate separators from the string
  string doubleSeparators = string(SEP) + string(SEP);
  int doubleSep = path.find(doubleSeparators);

  while (doubleSep != string::npos) {
    path.erase(doubleSep, 1);
    doubleSep = path.find(doubleSeparators);
  }

  int pos, firstSep = path.find(string(SEP) + "..");

  // if we find ".." we go backwards in the path, for example "F:\test\.."
  // becomes "F:\"
  while (firstSep != string::npos) {
    pos = path.find_last_of(SEP, firstSep - 1);

    if (pos == string::npos) {
      return "invalid";
    }

    path.erase(pos, firstSep - pos + 3);
    firstSep = path.find(string(SEP) + "..");
  }

  if (path.substr(path.size() - 1) == SEP) {
    path.erase(path.size() - 1);
  }

  // if the final path is the root add a separator at the end
  if (path == "" || (path.back() == ':' && path.size() == 2)) {
    path.append(SEP);
  }

  return path;
}

bool isValidFilename(string path) {
  // check for invalid symbols in the path
  string invalidSymbols = ILLEGAL_SYMBOLS;

  for (int i = 0; i < invalidSymbols.size(); i++) {
    if (path.find(invalidSymbols[i]) != path.npos) {
      return false;
    }
  }
  return true;
}

bool isValidPath(string path) {
  // check if the path exists
  if (!fs::exists(path) || path == "invalid") {
    return false;
  }

  string folderName = path.erase(0, path.find_last_of(SEP) + 1);

  return isValidFilename(folderName);
}

string getSizeOfDrive(string path) {
  fs::space_info size = fs::space(path);
  return compressSize(size.free) + " free of " + compressSize(size.capacity);
}

string getCurrentFolder(string path) {
  if (path == getDefaultPath()) {
    return getDefaultPath();
  }

  string oldPath = path;
  path = path.erase(0, path.find_last_of(SEP) + 1);

  // if the path is empty after erase, it means we are at the base point
  // windows: "C:\", linux: "/"
  if (path == "") {
    return oldPath.substr(0, oldPath.find(SEP));
  }

  return path;
}

void openEntry(string &path, string name, string ext) {
  if (!ext.empty()) {
    openFile(path, name, ext);
  } else
    openFolder(path, name);
}

void openFile(string path, string name, string ext) {
  if (!isValidPath(path))
    return;

  string command;
  string file = path;
  string app;

  if (file.back() != SEP[0])
    file.append(SEP);

  file += name;

  // add extension only if there is one
  if (ext.size() > 0) {
    file += "." + ext;
  }

  command =
      SEP == "\\" ? "start \"\" \"" + file + "\"" : "xdg-open \"" + file + "\"";

  system(command.c_str());
}

// goes into the next folder
void openFolder(string &path, string name) {
  string separator = "";

  // if path.back() is SEP, it means we are at the base point
  // windows: "C:\", linux: "/"
  if (path.back() != SEP[0]) {
    separator = SEP;
  }

  // if name is .., go back one folder
  if (name.compare("..") == 0) {
    if (path.back() == SEP[0]) {
      return;
    }

    // remove the last foldername from the path
    path = path.erase(path.find_last_of(separator), path.npos);

    // if the path reaches it's base point, append a separator
    // windows: "C:", linux: ""
    if (path.back() == ':' || path == "") {
      path.append(separator);
    }

    return;
  }

  string newPath = path;
  if (newPath.back() != SEP[0]) {
    newPath += SEP + name;

  } else {
    newPath += name;
  }

  // check if either path or new path is valid
  if (!isValidPath(path) || !isValidPath(newPath)) {
    return;
  }

  // check if the new path is a directory
  if (!fs::is_directory(newPath)) {
    return;
  }

  // update path to new path
  path = newPath;
}

// creates a new folder
void createFolder(string path, string name) {
  // checks if the folder already exists, case in which we add "(counter)" at
  // the end of the name
  if (!isValidPath(path))
    return;
  string folderName = name;
  int counter = 0;

  while (isValidPath(path + SEP + name)) {
    name = folderName + " (" + int2str(++counter) + ")";
  }

  // creates a folder with the specified name
  fs::create_directory(path + SEP + name);
}

// checks which function should be called based on entry
void copyEntry(string fromPath, string toPath) {
  if (fs::is_directory(fromPath)) {
    copyFolder(fromPath, toPath);
  } else
    copyFile(fromPath, toPath);
}

// copies a file from a path to another path
void copyFile(string fromPath, string toPath) {
  // checks if the file to be copied exists
  if (!isValidPath(fromPath) || !isValidPath(toPath)) {
    return;
  } else {
    if (fs::is_directory(fromPath) || !fs::is_directory(toPath)) {
      return;
    }
  }

  // initialize files and buffer
  FILE *fromPtr = fopen(fromPath.c_str(), "rb"), *toPtr;
  char buffer[1024];
  size_t bytes;

  int lastDot = fromPath.find_last_of("."),
      lastSep = fromPath.find_last_of(SEP[0]);
  string name = fromPath.substr(lastSep + 1, lastDot - lastSep - 1);
  // saves the extension of the file
  string extension = fromPath.substr(lastDot);

  if (toPath.back() != SEP[0]) {
    toPath.append(SEP);
  }

  toPath.append(name);

  // checks if a file already exists with the same name and extension, in
  // which case we add "(counter)" at the end
  string doesExist = toPath;
  int counter = 0;

  while (fs::exists((toPath + extension))) {
    toPath = doesExist + " (" + int2str(++counter) + ")";
  }

  toPath.append(extension);
  // // opens the file from "toPath" in write binary mode
  // toPath = toPath + extension;
  toPtr = fopen(toPath.c_str(), "wb");

  // copies the file from "fromPath" byte by byte to the file from "toPath"
  // and then closes the files
  while (0 < (bytes = fread(buffer, 1, sizeof(buffer), fromPtr))) {
    fwrite(buffer, 1, bytes, toPtr);
  }

  fclose(fromPtr);
  fclose(toPtr);
}

// copies a folder and its components from a path to another path
void copyFolder(string fromPath, string toPath) {
  // checks if the folder to be copied exists
  if (!isValidPath(fromPath) || !isValidPath(toPath)) {
    return;
  } else {

    if (!fs::is_directory(fromPath) || !fs::is_directory(toPath)) {
      return;
    }
  }

  // checks if the path where the folder is to be copied is valid
  if (toLower(evalPath(toPath)).find(toLower(evalPath(fromPath))) !=
      string::npos) {
    return;
  }

  // appends the folder name to the toPath string
  string path = fromPath;
  if (toPath.back() != SEP[0]) {
    toPath.append(SEP);
  }
  toPath.append(path.erase(0, path.find_last_of(SEP[0]) + 1));

  // creates the folder in the path stored in toPath, but checks if it already
  // exists, case in which it generates with the format "name (x)", where x is
  // counting how many copies there are already with the same format
  string name = toPath;
  name = name.erase(0, name.find_last_of(SEP) + 1);
  toPath = toPath.erase(toPath.find_last_of(SEP), toPath.npos);
  string folderName = name;
  int counter = 0;
  while (isValidPath(toPath + SEP + name)) {
    name = folderName + " (" + int2str(++counter) + ")";
  }

  // checks if the path is the base drive, case in which appends the separator
  // if it is missing
  if (toPath == "" || toPath.back() == ':') {
    toPath.append(SEP);
  }

  createFolder(toPath, name);
  if (toPath.back() != SEP[0]) {
    toPath += SEP;
  }
  toPath += name;

  // iterates in the current directory and if the entry is a directory we
  // recall the function recursively and when the entry is a file we copy it
  for (const auto &entry : fs::directory_iterator(fromPath)) {
    fs::path directoryPath = entry.path().filename();
    string filename = directoryPath.generic_string();

    if (entry.is_directory()) {
      copyFolder(fromPath + SEP + filename, toPath);
    } else {
      copyFile(fromPath + SEP + filename, toPath);
    }
  }
}

// checks which function should be called based on entry
void deleteEntry(string path) {
  if (fs::is_directory(path)) {
    deleteFolder(path);
  } else
    deleteFile(path);
}

// deletes a file from a specified path
void deleteFile(string path) {
  if (isValidPath(path) && !fs::is_directory(path)) {
    fs::remove(path);
  }
}

// deletes a folder from a specified path and its components
void deleteFolder(string path) {
  // checks if path exists
  if (!isValidPath(path) || !fs::is_directory(path)) {
    return;
  }

  for (const auto &entry : fs::directory_iterator(path)) {
    fs::path directoryPath = entry.path().filename();
    string filename = directoryPath.generic_string();

    if (entry.is_directory())
      deleteFolder(path + SEP + filename);

    deleteFile(path + SEP + filename);
  }

  fs::remove(path);
}

// checks which function should be called
void moveEntry(string fromPath, string toPath) {
  if ((fromPath.substr(0, fromPath.find_last_of(SEP)))
          .compare(evalPath(toPath)) == 0) {
    return;
  }
  if (fs::is_directory(fromPath)) {
    moveFolder(fromPath, toPath);
  } else
    moveFile(fromPath, toPath);
}
// moves a file from a path to another path
void moveFile(string fromPath, string toPath) {
  // checks if fromPath is equivalent to toPath (adding a SEP and using
  // evalPath in case the given string in toPath ends in a separator)

  copyFile(fromPath, toPath);
  deleteFile(fromPath);
}

// moves a folder from a path to another path
void moveFolder(string fromPath, string toPath) {
  // checks if the path where the folder is to be moved is valid
  if (toLower(evalPath(toPath)).find(toLower(evalPath(fromPath))) !=
      string::npos) {
    return;
  }

  copyFolder(fromPath, toPath);
  deleteFolder(fromPath);
}

// checks which function should be called
void editEntryName(string path, string newName) {
  if (fs::is_directory(path)) {
    editFolderName(path, newName);
  } else
    editFileName(path, newName);
}
// renames a file
void editFileName(string path, string newName) {
  // used for checking  if the name after the rename should stay the same as
  // the old one
  string oldFilename =
      path.substr(path.find_last_of(SEP) + 1,
                  path.find_last_of(".") - path.find_last_of(SEP) - 1);

  if (!isValidPath(path) || fs::is_directory(path)) {
    return;
  }

  string folder = path.substr(0, path.find_last_of(SEP) + 1);
  string ext = path.substr(path.find_last_of("."));
  string oldName = newName;
  int counter = 0;
  while (isValidPath(folder + SEP + newName + ext) && oldFilename != newName) {
    newName = oldName + " (" + int2str(++counter) + ")";
  }

  rename(path.c_str(), (folder + newName + ext).c_str());
}

// renames a folder
void editFolderName(string path, string newName) {
  // used for checking if the name after the rename should stay the same as
  // the old one
  string oldFoldername = path.substr(path.find_last_of(SEP) + 1);

  if (!isValidPath(path) || !fs::is_directory(path)) {
    return;
  }

  string folder = path.substr(0, path.find_last_of(SEP) + 1);
  string oldName = newName;
  int counter = 0;
  while (isValidPath(folder + SEP + newName) && oldFoldername != newName) {
    newName = oldName + " (" + int2str(++counter) + ")";
  }

  rename(path.c_str(), (folder + newName).c_str());
}
