#include "Staff.h"

#include "../../Providers/OrderProvider/OrderProvider.h"

////////////////////////////////////////////////////////////////
// Constructors and Destructor

Staff::Staff() : Seller(STAFF) {
  _baseSalary = STAFF_BASE_SALARY;
  _bonus = 0;
}

Staff::Staff(const Seller* seller) : Seller(*seller) {
  _baseSalary = STAFF_BASE_SALARY;
  _bonus = 0;
  _vouchers.clear();
  setRole(STAFF);
}

Staff::Staff(string username, wstring name, string phoneNumber, wstring address, ullong balance, list<string> vouchers)
    : Seller(username, name, phoneNumber, address, balance, vouchers, STAFF) {
  _baseSalary = STAFF_BASE_SALARY;
  _bonus = 0;
}

////////////////////////////////////////////////////////////////
// Getters and Setters

ullong Staff::baseSalary() const {
  return _baseSalary;
}

ullong Staff::bonus() const {
  return _bonus;
}

ullong Staff::salary() const {
  return _baseSalary + _bonus;
}

json Staff::getOwnInfor() const {
  json infor = Seller::getOwnInfor();

  infor["baseSalary"] = _baseSalary;
  infor["bonus"] = _bonus;

  return infor;
}

void Staff::setOwnInfor(json data) {
  Seller::setOwnInfor(data);

  ullong baseSalary = data["baseSalary"];
  ullong bonus = data["bonus"];

  _baseSalary = baseSalary;
  _bonus = bonus;
}

////////////////////////////////////////////////////////////////
// Manipulation methods

void Staff::resetBonus() {
  _bonus = 0;
}

// Accept an order
void Staff::acceptOrder(const string& orderID) {
  Member::acceptOrder(orderID);

  Order* order = OrderProvider::getInstance()->getOrder(orderID);

  // When Staff accept the order, bonus money will be added to the Staff's balance
  _bonus += 1.0 * order->saleCost() * STAFF_BONUS;

  _soldQuantity += order->quantity();
  _revenue += order->saleCost();

  // The base salary will be increase if Staff has sold an amount of products
  if (_soldQuantity % STAFF_SOLD_PIVOT == 0) {
    _baseSalary += STAFF_UP_SALARY;
  }
}
