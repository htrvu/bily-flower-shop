#include "Buyer.h"

#include "../../Providers/OrderProvider/OrderProvider.h"

////////////////////////////////////////////////////////////////
// Constructors and Destructor

Buyer::Buyer() : Member("", L"", "", L"", 0, BUYER) {
  _cart = new Cart;
}

Buyer::Buyer(string username, wstring name, string phoneNumber, wstring address, ullong balance)
    : Member(username, name, phoneNumber, address, balance, BUYER) {
  _cart = new Cart;
}

Buyer::Buyer(const Buyer& other) : Member(other) {
  _cart = new Cart(*other._cart);
}

Buyer::~Buyer() {
  delete _cart;
}

////////////////////////////////////////////////////////////////
// Operators

// Asignment operator
Buyer& Buyer::operator=(const Buyer& other) {
  Member::operator=(other);

  delete _cart;
  _cart = new Cart(*other._cart);

  return *this;
}

////////////////////////////////////////////////////////////////
// Getters and Setters

Cart* Buyer::cart() const {
  return _cart; 
}

json Buyer::getOwnInfor() const {
  json infor = Member::getOwnInfor();

  infor["cart"]["items"] = _cart->items();
  infor["cart"]["totalQuantity"] = _cart->totalQuantity();

  return infor;
}

void Buyer::setOwnInfor(json data) {
  Member::setOwnInfor(data);

  list<pair<string, uint>> items = data["cart"]["items"];
  uint totalQuantity = data["cart"]["totalQuantity"];

  _cart->setCart(items, totalQuantity);
}

////////////////////////////////////////////////////////////////
// Manipulation methods

// The function will run if an order of buyer is removed (or rejected)
void Buyer::removeOrder(const string& orderID, bool status) {
  Member::removeOrder(orderID, status);

  // If this order is already accepted, nothing need to be done now
  if (status == true) {
    return;
  }

  // If this order is rejected and the buyer chose to pay with their wallet, the
  // money they spent will be refunded
  Order* order = OrderProvider::getInstance()->getOrder(orderID);
  if (order->payWithWallet()) {
    ullong totalCost = order->saleCost();
    this->wallet()->deposit(totalCost);
  }

  return;
}
