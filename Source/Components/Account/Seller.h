#pragma once

#include "Member.h"

#include <list>
#include <vector>

class Seller : public Member {
private:
  list<string> _products;  // string: ID
  vector<uint> _ratings = {0, 0, 0, 0, 0};
protected:
  uint _soldQuantity;
  ullong _revenue;
  list<string> _vouchers;

public:
  Seller(string role = SELLER);
  Seller(string username, wstring name, string phoneNumber, wstring address, ullong balance = 0, list<string> vouchers = {}, string role = SELLER);

public:
  list<string> products();
  vector<uint> ratings();
  uint soldQuantity();
  ullong revenue();
  list<string> vouchers();
  virtual json getOwnInfor() const;
  virtual void setOwnInfor(json data);

public:
  virtual void acceptOrder(const string& orderID);
  void addProduct(const string& productID);
  void removeProduct(const string& productID);
  void addVoucher(const string& voucherID);
  void removeVoucher(const string& voucherID);
  void addRating(uint rating);
  double avgRating();
};
