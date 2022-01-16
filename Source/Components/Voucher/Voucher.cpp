#include "Voucher.h"

////////////////////////////////////////////////////////////////
// Constructors

Voucher::Voucher() {
  _ID = "";
  _name = L"";
  _salePercent = 0;
  _sellerUsername = "";
}

Voucher::Voucher(const string& ID, const wstring& name, double salePercent, const string& sellerUsername, list<string> productIDs) {
  _ID = ID;
  _name = name;
  _salePercent = salePercent;
  _sellerUsername = sellerUsername;
  _productIDs = productIDs;
}

////////////////////////////////////////////////////////////////
// Getters and Setters

string Voucher::ID() const {
  return _ID;
}

wstring Voucher::name() const {
  return _name;
}

double Voucher::salePercent() const {
  return _salePercent;
}

list<string> Voucher::productIDs() const {
  return _productIDs;
}

json Voucher::getFullInfor() const {
  json infor;

  infor["salePercent"] = _salePercent;
  infor["sellerUsername"] = _sellerUsername;
  infor["name"] = utf8(_name);
  infor["productIDs"] = _productIDs;

  return infor;
}

void Voucher::setID(const string& ID) {
  _ID = ID;
}

string Voucher::sellerUsername() const {
  return _sellerUsername;
}

////////////////////////////////////////////////////////////////
// Manipulation methods

bool Voucher::isEvent() const {
  return _productIDs.size() == 0;
}

void Voucher::editInfor(Voucher* newVoucherInfor) {
  _name = newVoucherInfor->name();
  _salePercent = newVoucherInfor->salePercent();
  _productIDs = newVoucherInfor->productIDs();
}

// Return true if this voucher is EVENT or _productIDs is not empty after removing
bool Voucher::removeProduct(const string &productID) {
  if (_productIDs.size() == 0) {
    return true;
  }

  _productIDs.remove(productID);
  return _productIDs.size() != 0;
}

