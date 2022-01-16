#include "Date.h"

////////////////////////////////////////////////////////////////
// Constructors and Destructor

// Default constructor
Date::Date() {
  setCurrentDate();
}

// Parameter constructor
Date::Date(uint day, uint month, uint year) {
  if (!isValidDate(day, month, year))
    setCurrentDate();
  else {
    _day = day;
    _month = month;
    _year = year;
  }
}

////////////////////////////////////////////////////////////////
// Getters and Setters

uint Date::day() const {
  return _day;
}

uint Date::month() const {
  return _month;
}

uint Date::year() const {
  return _year;
}

void Date::setDate(uint day, uint month, uint year) {
  if (!isValidDate(day, month, year)) {
    setCurrentDate();
  } else {
    _day = day;
    _month = month;
    _year = year;
  }
}

////////////////////////////////////////////////////////////////
// Manipulation methods

bool Date::isValid() {
  return isValidDate(_day, _month, _year);
}

// Get real Date
void Date::setCurrentDate() {
  time_t now = time(0);
  tm* ltm = localtime(&now);

  _day = ltm->tm_mday;
  _month = 1 + ltm->tm_mon;
  _year = 1900 + ltm->tm_year;
}

string Date::toString() {
  return zeroPrefix(_day) + "/" + zeroPrefix(_month) + "/" + zeroPrefix(_year);
}

////////////////////////////////////////////////////////////////
// Static methods

bool Date::isLeapYear(uint year) {
  return (year % 100 == 0) ? (year % 400 == 0) : (year % 4 == 0);
}

bool Date::isValidDate(uint day, uint month, uint year) {
  if (day <= 0 || month <= 0 || day > 31 || month > 12 || year <= 0)
    return false;

  if (month == 2)
    return (isLeapYear(year)) ? (day <= 29) : (day <= 28);

  return day <= 31 - (month == 4 || month == 6 || month == 9 || month == 11);
}
