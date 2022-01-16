#pragma once

#include <iostream>
#include <sstream>

#include "../../CONSTANTS.h"
#include "../../LIBRARIES.h"
#include "../Date/Date.h"

class Order {
private:
  string _ID;
  string _sellerUsername;
  string _buyerUsername;
  string _productID;
  uint _quantity;
  ullong _originalCost;
  ullong _saleCost;
  bool _isInEvent;
  bool _status;
  bool _isRated;
  bool _payWithWallet;
  Date* _date;
  wstring _address;
  wstring _note;

public:
  Order();
  Order(string ID, string sellerUsername, string buyerUsername, string productID, uint quantity,
        ullong originalCost, ullong saleCost, bool isInEvent, bool status, bool isRated,
        bool payWithWallet, Date* date, wstring address, wstring note);
  Order(const Order& other);
  Order& operator=(const Order& other);
  virtual ~Order();

public:
  string ID() const;
  string sellerUsername() const;
  string buyerUsername() const;
  string productID() const;
  uint quantity() const;
  ullong originalCost() const;
  ullong saleCost() const;
  bool isInEvent() const;
  bool status() const;
  bool isRated() const;
  bool payWithWallet() const;
  Date* date() const;
  wstring address() const;
  wstring note() const;
  json getFullInfor() const;

  void setID(const string& ID);
  void setStatus(bool status);
  void setIsRated(bool isRated);
};
