#pragma once

#include "../../LIBRARIES.h"
#include "../../CONSTANTS.h"

#include <list>

class Cart {
private:
  list<pair<string, uint>> _items;  // <productID, quantity>
  uint _totalQuantity;

public:
  Cart();

public:
  list<pair<string, uint>> items() const;
  uint totalQuantity() const;
  void setCart(const list<pair<string, uint>>& items, uint totalQuantity);
  
public:
  ullong totalCost();
  void addItem(const string& productID, uint quantity);
  bool removeItem(const string& productID);
  void changeQuantity(const string& productID, uint newQuantity);
  void clear();
};