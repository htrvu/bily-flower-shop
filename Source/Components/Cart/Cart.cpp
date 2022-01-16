#include "Cart.h"

#include "../../Providers/ProductProvider/ProductProvider.h"

////////////////////////////////////////////////////////////////
// Constructor

Cart::Cart() {
  _totalQuantity = 0;
}

////////////////////////////////////////////////////////////////
// Getters and Setters

list<pair<string, uint>> Cart::items() const {
  return _items;
}

uint Cart::totalQuantity() const {
  return _totalQuantity;
}

void Cart::setCart(const list<pair<string, uint>>& items, uint totalQuantity) {
  _items = items;
  _totalQuantity = totalQuantity;
}

////////////////////////////////////////////////////////////////
// Manipulation methods

// Calculate the total cost of items in Cart
ullong Cart::totalCost() {
  ullong totalCost = 0;
  ProductProvider* productProvider = ProductProvider::getInstance();

  for (auto& item : _items) {
    Product* product = productProvider->getProduct(item.first);
    totalCost += product->salePrice() * item.second;
  }
  
  return totalCost;
}

// Add items to Cart
void Cart::addItem(const string& productID, uint quantity) {
  // if item is already in Cart, add quantity
  bool exist = false;
  for (auto i = _items.begin(); i != _items.end(); i++) {
    if (i->first == productID) {
      i->second += quantity;
      exist = true;
      break;
    }
  }

  _totalQuantity += quantity;

  // If item is not in Cart, add item
  if (!exist) {
    _items.push_back(make_pair(productID, quantity));
  }
}

// Delete item from Cart
bool Cart::removeItem(const string& productID) {
  for (auto i = _items.begin(); i != _items.end(); i++) {
    if (i->first == productID) {
      uint quantity = i->second;
      _totalQuantity -= quantity;

      _items.erase(i);

      return true;
    }
  }

  // If item is not in Cart, return false
  return false;
}

// Change quantity of item in Cart
void Cart::changeQuantity(const string& productID, uint newQuantity) {
  for (auto i = _items.begin(); i != _items.end(); i++) {
    if (i->first == productID) {
      _totalQuantity -= i->second;

      i->second = newQuantity;
      _totalQuantity += newQuantity;
      
      return;
    }
  }
}

// Clear the cart
void Cart::clear() {
  _items.clear();
  _totalQuantity = 0;
}
