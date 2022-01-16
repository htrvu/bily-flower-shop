#pragma once

#include <set>
#include <vector>

#include "../../CONSTANTS.h"
#include "../../LIBRARIES.h"

class Product {
private:
  string _ID;
  uint _category;
  wstring _name;
  string _sellerUsername;
  wstring _description;
  ullong _originalPrice;
  ullong _salePrice;
  multiset<pair<float, int>> _discounts;
  uint _stock;
  uint _sold;
  vector<uint> _ratings = {0, 0, 0, 0, 0};
  string _imageDir;

public:
  Product();
  Product(string ID, uint category, wstring name, string sellerUsername, wstring description, ullong originalPrice,
          ullong salePrice, multiset<pair<float, int>> discounts, uint stock, uint _sold,
          vector<uint> ratings, string imageDir);
  Product(uint category, wstring name, string sellerUsername, wstring description, ullong originalPrice,
          uint stock, string imageDir);

public:
  string ID() const;
  uint category() const;
  wstring name() const;
  string sellerUsername() const;
  wstring description() const;
  ullong originalPrice() const;
  ullong salePrice() const;
  multiset<pair<float, int>> discounts() const;
  uint stock() const;
  uint sold() const;
  vector<uint> ratings() const;
  float avgRating() const;
  string imageDir() const;
  json getFullInfor() const;

public:
  void setID(const string& ID);
  void editInfor(Product* newProductInfor);
  void addRating(uint rating);
  float getDiscount() const;
  bool isInEvent() const;
  void addDiscount(float discount, int type);
  void removeDiscount(float discount, int type);
  void changeSoldQuantity(int amount);
  void changeStockQuantity(int amount);
};
