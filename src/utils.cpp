#include "utils.h"

// converts a string to an integer
int str2int(string stringNumber) {
  int number = 0;

  for (int i = 0; i < stringNumber.size(); i++) {
    if (stringNumber[i] == ',') {
      continue;
    }

    if ('0' <= stringNumber[i] && stringNumber[i] <= '9') {
      number = number * 10 + int(stringNumber[i] - '0');
    } else {
      return -1;
    }
  }

  return number;
}

// converts an integer to string
string int2str(int a) {
  if (a == -1) {
    return "<DIR>";
  }

  string stringNumber;

  while (a) {
    stringNumber.insert(stringNumber.begin(), char(a % 10 + '0'));
    a /= 10;
  }

  return stringNumber;
}

// parses a string into a Filedata structure
Filedata parseFileDataString(string filedataString) {
  // remove extra spaces from string
  for (int i = 1; i < filedataString.size(); i++) {
    if (filedataString[i] == filedataString[i - 1] &&
        filedataString[i] == ' ') {
      filedataString.erase(i--, 1);
    }
  }

  // the structure of the file data string is:
  // DATE TIME AM/PM <DIR>/SIZE NAME
  Filedata filedata;

  int spacePos = -1;

  for (int i = 0; i < 3; i++) {
    spacePos = filedataString.find(' ', spacePos + 1);
  }

  filedata.date = filedataString.substr(0, spacePos);

  int nextSpace = filedataString.find(' ', spacePos + 1);

  filedata.size =
      filedataString.substr(spacePos + 1, nextSpace - (spacePos + 1));

  spacePos = nextSpace;

  filedata.filename = filedataString.substr(
      nextSpace + 1, filedataString.size() - (nextSpace + 1));

  int lastDotPos = filedata.filename.find_last_of('.');

  if (lastDotPos != string::npos && filedata.size.compare("<DIR>")) {
    filedata.ext = filedata.filename.substr(lastDotPos + 1);
    filedata.filename.erase(filedata.filename.find_last_of('.'));
  } else
    filedata.ext = "";

  return filedata;
}

// this function will convert strings that of format "Sun Nov 17 13:01:15 2022"
// to "17/11/2022 01:01 PM"
string formatDate(string date) {
  // gets rid of the first 4 characters (day of the week)
  date.erase(0, 4);

  string time = date.substr(7, 5);
  string day = date.substr(4, 2);
  string month = date.substr(0, 3);
  string year = date.substr(16, 4);
  string months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                       "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

  // converts month from word to number
  for (int i = 0; i < 12; i++) {
    if (month.compare(months[i]) == 0) {
      month = i < 9 ? "0" : "";
      month += int2str(i + 1);
      break;
    }
  }

  // converts normal hour format to AM/PM
  string hour = time.substr(0, 2);
  string hourFormat = hour.compare("12") > 0 ? "PM" : "AM";

  if (hourFormat.compare("PM") == 0) {
    int hourInteger = str2int(hour) - 12;
    if (hourInteger < 10) {
      hour = "0";
    }
    hour = int2str(hourInteger);
    time.erase(0, 2);
    time = hour + time;
  }

  // returns the formatted string
  return day + "/" + month + "/" + year + " " + time + " " + hourFormat;
}