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

string compressSize(uintmax_t sizeToCompress) {
  string dim = " KMGT";
  double doubleSize = sizeToCompress;
  unsigned long long intSize = 0;
  int dimIterator = 0;

  while (doubleSize > 999) {
    doubleSize /= 1024;
    intSize = (unsigned long long)(doubleSize * 10);
    dimIterator++;
  }

  string driveSize = uint2str(intSize / 10);
  string dimLetter = "";

  if (dim[dimIterator] != ' ') {
    dimLetter = dim[dimIterator];
  }
  return driveSize + "." + uint2str(intSize % 10) + " " + dimLetter + "B";
}

// converts an integer to string
string uint2str(uintmax_t a) {
  if (a == -1) {
    return "<DIR>";
  } else if (a == 0) {
    return "0";
  }

  string stringNumber;

  while (a) {
    stringNumber.insert(stringNumber.begin(), char(a % 10 + '0'));
    a /= 10;
  }

  return stringNumber;
}

string int2str(int a) {
  if (a == -1) {
    return "<DIR>";
  } else if (a == 0) {
    return "0";
  }

  string stringNumber;

  while (a) {
    stringNumber.insert(stringNumber.begin(), char(a % 10 + '0'));
    a /= 10;
  }

  return stringNumber;
}

string toLower(string s) {
  for (int i = 0; i < s.size(); i++) {
    if ('A' <= s[i] && s[i] <= 'Z') {
      s[i] = s[i] - 'A' + 'a';
    }
  }

  return s;
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

  // if day is lower than 10, append a 0
  if (day[0] == ' ') {
    day[0] = '0';
  }

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
    hour = (hourInteger < 10 ? "0" : "") + int2str(hourInteger);

    time.erase(0, 2);
    time = hour + time;
  }

  // returns the formatted string
  return day + "/" + month + "/" + year + " " + time + " " + hourFormat;
}
