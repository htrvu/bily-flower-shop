#pragma once

#include <list>

#include "Account.h"
#include <iostream>

// Singleton design pattern

class Admin : public Account {
private:
  list<string> _staffs;
  ullong _shopRevenue;
  long long _profitFromSeller;

private:
  Admin();
  Admin(string username, wstring name, string phoneNumber, wstring address,
        ullong balance, ullong shopRevenue, long long profitFromSeller);

public:
  static Admin* getInstance(string username = "", wstring name = L"",
                            string phoneNumber = "", wstring address = L"",
                            ullong balance = 0, ullong shopRevenue = 0,
                            long long profitFromSeller = 0);
  ~Admin() {}

public:
  list<string> staffs() const;
  ullong shopRevenue() const;
  long long profitFromSeller() const;
  json getOwnInfor() const;
  void setOwnInfor(json data);

public:
  ullong totalSalaries();
  bool paySalaries();
  void addStaff(const string& userName);
  void dismissStaff(const string& userName);
  void addShopRevenue(ullong amount);
  void addProfitFromSeller(long long amount);
};
