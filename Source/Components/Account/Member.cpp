#include "Member.h"

#include "../../Providers/OrderProvider/OrderProvider.h"

////////////////////////////////////////////////////////////////
// Constructors and Destructor

Member::Member() : Account() {}

Member::Member(string username, wstring name, string phoneNumber, wstring address, ullong balance, string role)
    : Account(username, name, phoneNumber, address, balance, role) {}

Member::~Member() {}

////////////////////////////////////////////////////////////////
// Getters and Setters

list<string> Member::acceptedOrders() {
  return _acceptedOrders;
}

list<string> Member::pendingOrders() {
  return _pendingOrders;
}

json Member::getOwnInfor() const {
  json infor;

  infor["acceptedOrders"] = _acceptedOrders;
  infor["pendingOrders"] = _pendingOrders;

  return infor;
}

void Member::setOwnInfor(json data) {
  list<string> acceptedOrders = data["acceptedOrders"];
  list<string> pendingOrders = data["pendingOrders"];
  _acceptedOrders = acceptedOrders;
  _pendingOrders = pendingOrders;
}

////////////////////////////////////////////////////////////////
// Manipulation methods

void Member::addOrder(const string& orderID) {
  _pendingOrders.push_back(orderID);
}

void Member::acceptOrder(const string& orderID) {
  _acceptedOrders.push_back(orderID);
  _pendingOrders.remove(orderID);
}

void Member::removeOrder(const string& orderID, bool status) {
  if (status == true) {
    _acceptedOrders.remove(orderID);
  } else {
    _pendingOrders.remove(orderID);
  }
}
