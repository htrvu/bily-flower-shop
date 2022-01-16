#pragma once
#include "Member.h"
#include "../Cart/Cart.h"

class Buyer: public Member {
private:
  Cart* _cart;

public:
  Buyer();
  Buyer(string username, wstring name, string phoneNumber, wstring address, ullong balance = 0);
  Buyer(const Buyer& other);
  ~Buyer();

public:
  Buyer& operator=(const Buyer& other);

public:
  Cart* cart() const;
  json getOwnInfor() const;
  void setOwnInfor(json data);

public:
  void removeOrder(const string& orderID, bool status);
};
