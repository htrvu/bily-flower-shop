#include "Order.h"

////////////////////////////////////////////////////////////////
// Constructors and Destructor

// Default constructor
// Date: get the date at present
Order::Order() {
  _ID = _sellerUsername = _buyerUsername = _productID = "";
  _quantity = _originalCost = _saleCost = 0;
  _isInEvent = _status = _isRated = _payWithWallet = 0;
  _address = _note = L"";
  _date = new Date();
}

// Parameter constructor
Order::Order(string ID, string sellerUsername, string buyerUsername, string productID,
             uint quantity, ullong originalCost, ullong saleCost, bool isInEvent, bool status,
             bool isRated, bool payWithWallet, Date* date, wstring address, wstring note) {
  _ID = ID;
  _sellerUsername = sellerUsername;
  _buyerUsername = buyerUsername;
  _productID = productID;
  _quantity = quantity;
  _originalCost = originalCost;
  _saleCost = saleCost;
  _isInEvent = isInEvent;
  _status = status;
  _isRated = isRated;
  _payWithWallet = payWithWallet;
  _address = address;
  _note = note;
  _date = new Date(*date);
}

// Copy constructor
Order::Order(const Order& other) {
  _ID = other._ID;
  _sellerUsername = other._sellerUsername;
  _buyerUsername = other._buyerUsername;
  _productID = other._productID;
  _quantity = other._quantity;
  _originalCost = other._originalCost;
  _saleCost = other._saleCost;
  _isInEvent = other._isInEvent;
  _status = other._status;
  _payWithWallet = other._payWithWallet;
  _address = other._address;
  _note = other._note;
  _date = new Date(*other._date);
}

// Destructor
Order::~Order() {
  if (_date != nullptr) {
    delete _date;
    _date = nullptr;
  }
}

////////////////////////////////////////////////////////////////
// Operators

// Assignment operator
Order& Order::operator=(const Order& other) {
  if (&other == this) {
    return *this;
  }
  
  _ID = other._ID;
  _sellerUsername = other._sellerUsername;
  _buyerUsername = other._buyerUsername;
  _productID = other._productID;
  _quantity = other._quantity;
  _originalCost = other._originalCost;
  _saleCost = other._saleCost;
  _isInEvent = other._isInEvent;
  _status = other._status;
  _isRated = other._isRated;
  _payWithWallet = other._payWithWallet;
  _address = other._address;
  _note = other._note;
  *_date = *other._date;

  return *this;
}

////////////////////////////////////////////////////////////////
// Getters and Setters

string Order::ID() const {
  return _ID;
}

string Order::sellerUsername() const {
  return _sellerUsername;
}

string Order::buyerUsername() const {
  return _buyerUsername;
}

string Order::productID() const {
  return _productID;
}

uint Order::quantity() const {
  return _quantity;
}

ullong Order::originalCost() const {
  return _originalCost;
}

ullong Order::saleCost() const {
  return _saleCost;
}

bool Order::isInEvent() const {
  return _isInEvent;
}

bool Order::status() const {
  return _status;
}

bool Order::isRated() const {
  return _isRated;
}

bool Order::payWithWallet() const {
  return _payWithWallet;
}

Date* Order::date() const {
  return _date;
}

wstring Order::address() const {
  return _address;
}

wstring Order::note() const {
  return _note;
}

json Order::getFullInfor() const {
  json infor;

  infor["sellerUsername"] = _sellerUsername;
  infor["buyerUsername"] = _buyerUsername;
  infor["productID"] = _productID;
  infor["quantity"] = _quantity;
  infor["originalCost"] = _originalCost;
  infor["saleCost"] = _saleCost;
  infor["isInEvent"] = _isInEvent;
  infor["status"] = _status;
  infor["isRated"] = _isRated;
  infor["payWithWallet"] = _payWithWallet;
  infor["address"] = utf8(_address);
  infor["note"] = utf8(_note);
  vector<uint> dateArr = {_date->day(), _date->month(),
                          _date->year()};
  infor["date"] = dateArr;

  return infor;
}

void Order::setID(const string& ID) {
  _ID = ID;
}

void Order::setStatus(bool status) {
  _status = status;
}

void Order::setIsRated(bool isRated) {
  _isRated = isRated;
}