#pragma once

#include "Seller.h"

class Staff : public Seller {
private:
  ullong _baseSalary;
  ullong _bonus;

public:
  Staff();
  Staff(const Seller* seller);
  Staff(string username, wstring name, string phoneNumber, wstring address, ullong balance = 0, list<string> vouchers = {});

public:
  ullong baseSalary() const;
  ullong bonus() const;
  ullong salary() const;
  json getOwnInfor() const;
  void setOwnInfor(json data);

public:
  void resetBonus();
  void acceptOrder(const string& orderID);
};
