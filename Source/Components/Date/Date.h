#pragma once

#include "../../CONSTANTS.h"
#include "../../LIBRARIES.h"

#include <ctime>
#include <string>
using namespace std;

class Date {
private:
  uint _day;
  uint _month;
  uint _year;

public:
  Date();
  Date(uint day, uint month, uint year);

public:
  uint day() const;
  uint month() const;
  uint year() const;
  void setDate(uint day, uint month, uint year);

public:
  bool isValid();
  void setCurrentDate();
  string toString();

public:
  static bool isLeapYear(uint year);
  static bool isValidDate(uint day, uint month, uint year);
};
