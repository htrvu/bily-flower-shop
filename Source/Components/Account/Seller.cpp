#include "Seller.h"

#include "../../Providers/OrderProvider/OrderProvider.h"

///////////////////////////////////////////////////////////////////////////////
// Constructors

Seller::Seller(string role) : Member("", L"", role, L"", 0, role) {
  _soldQuantity = 0;
}

Seller::Seller(string username, wstring name, string phoneNumber, wstring address,ullong balance, list<string> vouchers, string role)
    : Member(username, name, phoneNumber, address, balance, role) {
  _vouchers = vouchers;
  _soldQuantity = 0;
}

///////////////////////////////////////////////////////////////////////////////
// Getters and Setters

list<string> Seller::products() {
  return _products;
}

vector<uint> Seller::ratings() {
  return _ratings;
}

uint Seller::soldQuantity() {
  return _soldQuantity;
}

ullong Seller::revenue() {
  return _revenue;
}

list<string> Seller::vouchers() {
  return _vouchers;
}

json Seller::getOwnInfor() const {
  json infor = Member::getOwnInfor();

  infor["items"] = _products;
  infor["ratings"] = _ratings;
  infor["soldQuantity"] = _soldQuantity;
  infor["revenue"] = _revenue;
  infor["vouchers"] = _vouchers;

  return infor;
}

void Seller::setOwnInfor(json data) {
  Member::setOwnInfor(data);

  list<string> products = data["items"];
  vector<uint> ratings = data["ratings"];
  uint soldQuantity = data["soldQuantity"];
  ullong revenue = data["revenue"];
  list<string> vouchers = data["vouchers"];

  _products = products;
  _ratings = ratings;
  _soldQuantity = soldQuantity;
  _revenue = revenue;
  _vouchers = vouchers;
}

///////////////////////////////////////////////////////////////////////////////
// Manipulation methods

// Accept an order
void Seller::acceptOrder(const string& orderID) {
  Member::acceptOrder(orderID);

  Order* order = OrderProvider::getInstance()->getOrder(orderID);

  _soldQuantity += order->quantity();
  
  ullong revenue = 0;
  // If there is a event applied to the order, the Seller (not Staff) will receive the originalCost
  // of that order. 
  if (order->isInEvent()) {
    revenue = order->originalCost();
  }
  // Ortherwise, the Seller will receive the saleCost of the order
  else {
    revenue = order->saleCost();
  }
  
  revenue *= (1 - ORDER_PROFIT);
  _revenue += revenue;
  this->wallet()->deposit(revenue);
}

// Add product to the product list of seller
void Seller::addProduct(const string& productID) {
  _products.push_back(productID);
}

// Remove product from the product list of seller
void Seller::removeProduct(const string& productID) {
  _products.remove(productID);
}

// Add voucher to the voucher list
void Seller::addVoucher(const string& voucherID) {
  _vouchers.push_back(voucherID);
}

// Remove voucher from the voucher list
void Seller::removeVoucher(const string& voucherID) {
  _vouchers.remove(voucherID);
}

// Add rating to the rating list
// <rating>: an integer that is in interval [1, 5]
void Seller::addRating(uint rating) {
  _ratings[rating - 1]++;
}

// Get the average rating this Seller received
double Seller::avgRating() {
  double sum = 0;
  uint count = 0;

  for (uint i = 0; i < _ratings.size(); i++) {
    sum += 1.0 * _ratings[i] * (i + 1);
    count += _ratings[i];
  }

  if (count == 0) return 0;
  return sum / count;
}
