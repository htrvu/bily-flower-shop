#pragma once

#include "Account.h"

#include <list>

class Member: public Account {
protected:
  list<string> _acceptedOrders; // string: ID of order
  list<string> _pendingOrders;  // string: ID of order

public:
  Member();
  Member(string username, wstring name, string phoneNumber, wstring address, ullong balance, string role);
  virtual ~Member();

public:
  list<string> acceptedOrders();
  list<string> pendingOrders();
  virtual json getOwnInfor() const;
  virtual void setOwnInfor(json data);

public:
  virtual void addOrder(const string& orderID);
  virtual void acceptOrder(const string& orderID);
  virtual void removeOrder(const string& orderID, bool status);
};
