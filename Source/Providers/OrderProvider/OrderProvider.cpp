#include "OrderProvider.h"

#include "../../Components/Account/Admin.h"
#include "../../Components/Account/Buyer.h"
#include "../../Components/Account/Seller.h"
#include "../AccountProvider/AccountProvider.h"
#include "../ProductProvider/ProductProvider.h"

///////////////////////////////////////////////////////////////////////////////////
// Constructor and Destructor

OrderProvider::OrderProvider() { readData(); }

// Get the instance of OrderProvider
OrderProvider* OrderProvider::getInstance() {
  static OrderProvider instance;
  return &instance;
}

OrderProvider::~OrderProvider() {
  writeData();

  for (auto i = _orders.begin(); i != _orders.end(); i++) {
    delete i->second;
    i->second = nullptr;
  }
}

///////////////////////////////////////////////////////////////////////////////////
// Manipulation methods

// Read the information of orders from "database"
void OrderProvider::readData() {
  json jsonData = readJSON(ORDERS_PATH);

  uint counter = jsonData["COUNTER"];
  _counter = counter;

  for (auto i = jsonData["ORDERS"].begin(); i != jsonData["ORDERS"].end(); ++i) {
    Order* order = nullptr;
    string orderID = i.key();
    json info = i.value();

    string sellerUsername = info["sellerUsername"];
    string buyerUsername = info["buyerUsername"];
    string productID = info["productID"];
    uint quantity = info["quantity"];
    ullong originalCost = info["originalCost"];
    ullong saleCost = info["saleCost"];
    bool isInEvent = info["isInEvent"];
    bool status = info["status"];
    bool isRated = info["isRated"];
    bool payWithWallet = info["payWithWallet"];
    wstring address = utf16(info["address"]);
    wstring note = utf16(info["note"]);

    vector<int> dateArr = info["date"];
    Date* date = new Date(dateArr[0], dateArr[1], dateArr[2]);

    order = new Order(orderID, sellerUsername, buyerUsername, productID, quantity, originalCost,
                      saleCost, isInEvent, status, isRated, payWithWallet, date, address, note);

    _orders[orderID] = order;
  }
}

// Write the information of orders to "database"
void OrderProvider::writeData() {
  json jsonData;

  jsonData["COUNTER"] = _counter;

  // json info;
  for (auto i : _orders) {
    string orderID = i.first;
    json infor = i.second->getFullInfor();
    jsonData["ORDERS"][orderID] = infor;
  }

  writeJSON(jsonData, ORDERS_PATH);
}

// Generate the ID for next order
string OrderProvider::nextID() {
  _counter++;
  string ID = "O" + to_string(_counter);

  return ID;
}

// Add an order to "database"
void OrderProvider::addOrder(Order* order) {
  string ID = nextID();
  order->setID(ID);

  _orders[order->ID()] = order;

  AccountProvider* accountProvider = AccountProvider::getInstance();
  Member* buyer = (Member*)accountProvider->getAccount(order->buyerUsername());
  Member* seller = (Member*)accountProvider->getAccount(order->sellerUsername());

  seller->addOrder(order->ID());
  buyer->addOrder(order->ID());
}

// Remove (or reject) an order from "database"
bool OrderProvider::removeOrder(const string& orderID) {
  if (_orders.find(orderID) == _orders.end()) {
    return false;
  }

  Order* order = _orders[orderID];
  bool status = order->status();
  if (status == false) {
    // Increase the stock quantity
    Product* product = ProductProvider::getInstance()->getProduct(order->productID());
    product->changeStockQuantity(order->quantity());
  }

  AccountProvider* accountProvider = AccountProvider::getInstance();
  Member* buyer = (Member*)accountProvider->getAccount(order->buyerUsername());
  Member* seller = (Member*)accountProvider->getAccount(order->sellerUsername());

  buyer->removeOrder(orderID, status);
  seller->removeOrder(orderID, status);

  delete order;
  _orders.erase(orderID);

  return true;
}

// Remove all orders which related to a product
void OrderProvider::removeOrdersByProduct(const string& productID) {
  for (auto i = _orders.begin(); i != _orders.end(); i++) {
    if (i->second->productID() == productID) {
      removeOrder(i->first);
    }
  }
}

// Accept an order
bool OrderProvider::acceptOrder(const string& orderID) {
  if (_orders.find(orderID) == _orders.end() || _orders[orderID]->status() == true) {
    return false;
  }

  Order* order = _orders[orderID];
  order->setStatus(true);

  // Increase the sold quantity
  Product* product = ProductProvider::getInstance()->getProduct(order->productID());
  product->changeSoldQuantity(order->quantity());

  AccountProvider* accountProvider = AccountProvider::getInstance();
  Member* buyer = (Member*)accountProvider->getAccount(order->buyerUsername());
  Member* seller = (Member*)accountProvider->getAccount(order->sellerUsername());
  Admin* admin = Admin::getInstance();

  // Admin's profit from this order
  // If this is a Seller's order:
  if (seller->role() == SELLER) {
    long long adminProfit = 0;
    // If there is a event applied to the order, Admin will pay for the difference between
    // the originalCost and the saleCost
    if (order->isInEvent()) {
      adminProfit = (long long)order->saleCost() - order->originalCost() * (1 - ORDER_PROFIT);
    }
    // Ortherwise, Admin will receive an amount of ORDER_PROFIT * saleCOst
    else {
      adminProfit = order->saleCost() * ORDER_PROFIT;
    }

    admin->addProfitFromSeller(adminProfit);
  } 
  // If this is a Staff's order
  else {
    // When Staff accept the order, Amin will receive an amount of saleCost * (1 - STAFF_BONUS)
    admin->addShopRevenue(1.0 * order->saleCost() * (1 - STAFF_BONUS));
  }

  // Polymophism
  buyer->acceptOrder(orderID);
  seller->acceptOrder(orderID);

  return true;
}

// Get the pointer to an order by ID
Order* OrderProvider::getOrder(const string& orderID) {
  if (_orders.find(orderID) == _orders.end()) return nullptr;

  return _orders[orderID];
}

// Checkout: create orders for products
// If "productID" is a empty string, we will create orders for all products in buyer's cart
// Ortherwise, we will create an order for the product with ID "productID"
// Return: a pair of an Error code and Data
//    Error code:   0: OK
//                  -1: the balance of wallet is not enough
//                  -2: the stock quantity of a product is not enough
//    Data:         the ID of product if this is a checkout for a specific product and ErrorCode is -2,
//                  "cart" if this is a checkout for all items in cart and errorCode is 0, otherwise ""
pair<int, string> OrderProvider::checkoutHandling(const string& buyerUsername, const string& productID,
                                     uint quantity, bool payWithWallet, const wstring& address,
                                     const wstring& note) {
  ProductProvider* productProvider = ProductProvider::getInstance();

  Buyer* buyer = (Buyer*)AccountProvider::getInstance()->getAccount(buyerUsername);
  list<pair<string, uint>> items;   // an item includes productID and quantity
  ullong totalCost = 0;
  bool isInCart = false;

  // Determine the buyer is checkout the items in cart or not
  if (productID.length() > 0) {
    items.push_back({productID, quantity});
    totalCost = productProvider->getProduct(productID)->salePrice() * quantity;
  } else {
    items = buyer->cart()->items();
    totalCost = buyer->cart()->totalCost();
    isInCart = true;
  }

  // If the buyer is using wallet to pay, check if the buyer has enough money and deducted it
  if (payWithWallet) {
    if (buyer->wallet()->balance() < totalCost) {
      return {-1, ""};
    }

    buyer->wallet()->withdraw(totalCost);
  }

  // If the stock quantity of any product is not enough, raise error
  for (auto i = items.begin(); i != items.end(); i++) {
    string productID = i->first;
    uint quantity = i->second;
    Product* product = productProvider->getProduct(productID);

    if (product->stock() < quantity) {
      return {-2, productID};
    }
  }

  // Make a new order for each item
  Date* date = new Date();

  for (auto i = items.begin(); i != items.end(); i++) {
    string productID = i->first;
    uint quantity = i->second;

    Product* product = productProvider->getProduct(productID);

    // Reduce the stock of the product
    product->changeStockQuantity(0 - quantity);

    string sellerUsername = product->sellerUsername();
    ullong originalCost = product->originalPrice() * quantity;
    ullong saleCost = product->salePrice() * quantity;
    bool status = false;
    bool isRated = false;
    bool isInEvent = product->isInEvent();

    Order* order = new Order("", sellerUsername, buyerUsername, productID, quantity, originalCost,
                             saleCost, isInEvent, status, isRated, payWithWallet, date, address, note);

    addOrder(order);
  }

  // Clear the cart of buyer (if this checkout is for all products in buyer's cart)
  if (isInCart) {
    buyer->cart()->clear();
  }

  delete date;
  return {0, isInCart ? "cart" : ""};
}

// Rate an order
void OrderProvider::rateOrder(string orderID, uint rating) {
  Order* order =_orders[orderID];

  if (order->isRated()) {
    return;
  }

  // Set the rating status of this order
  order->setIsRated(true);

  // Add this rating to the product's rating data
  string productID = order->productID();
  ProductProvider::getInstance()->getProduct(productID)->addRating(rating);

  // Add this rating to the seller's rating data
  string sellerUsername = order->sellerUsername();
  Seller* seller = (Seller*)(AccountProvider::getInstance()->getAccount(sellerUsername));
  seller->addRating(rating);
}
