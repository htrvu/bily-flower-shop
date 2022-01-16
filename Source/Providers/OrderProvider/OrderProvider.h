#pragma once

#include <unordered_map>

#include "../../CONSTANTS.h"
#include "../../Components/Order/Order.h"
#include "../../LIBRARIES.h"

class OrderProvider {
private:
  unordered_map<string, Order*> _orders;
  uint _counter;

private:
  OrderProvider();
  OrderProvider(const OrderProvider&) {}
  OrderProvider& operator=(const OrderProvider&) { return *this; }

public:
  static OrderProvider* getInstance();
  ~OrderProvider();

public:
  void readData();
  void writeData();

private:
  string nextID();
public:
  Order* getOrder(const string& orderID);
  void addOrder(Order* order);
  bool acceptOrder(const string& orderID);
  bool removeOrder(const string& orderID);
  void removeOrdersByProduct(const string& productID);
  pair<int, string> checkoutHandling(const string& buyerUsername, const string& productID, uint quantity,
                        bool payWithWallet, const wstring& address, const wstring& note);
  void rateOrder(string orderID, uint rating);
};
