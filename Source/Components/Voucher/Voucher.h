#pragma once

#include <list>

#include "../../CONSTANTS.h"
#include "../../LIBRARIES.h"

class Voucher {
private:
  string _ID;
  double _salePercent;
  wstring _name;
  string _sellerUsername;
  list<string> _productIDs; // {} if this is an "event"
                            // {"all"} or {"ID_1", "ID_2", ... } if this is a voucher
public:
  Voucher();
  Voucher(const string& ID, const wstring& name, double salePercent, const string& sellerUsername, list<string> productIDs = {});

public:
  string ID() const;
  wstring name() const;
  double salePercent() const;
  string sellerUsername() const;
  list<string> productIDs() const;
  json getFullInfor() const;

  void setID(const string& ID);

public:
  void editInfor(Voucher* newVoucherInfor);
  bool removeProduct(const string& productID);
  bool isEvent() const;
};
