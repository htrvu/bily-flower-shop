#pragma once

#include "../../LIBRARIES.h"
#include "../../CONSTANTS.h"

#include "../../Components/Product/Product.h"

#include <list>

class ProductProvider {
private:
  ProductProvider();
  ProductProvider(const ProductProvider&) {}
  ProductProvider& operator=(const ProductProvider&) { return *this; }

public:
  static ProductProvider* getInstance();
  ~ProductProvider();

private:
  vector<string> _categoryNames;
  unordered_map<string, Product*> _products;            // string: ID
  unordered_map<string, string> _searchingData;         // data for searching: productID and main content of that product
  unordered_map<string, list<string>> _categories;      // string: category, vector<string>: ID of products
  uint _counter;

public:
  void readData();
  void writeData();

private:
  string nextID();
public:
  string getCategoryName(uint index);
  void addProduct(Product* product);
  bool removeProduct(const string& productID);
  void changeCategory(const string& productID, uint oldCategory, uint newCategoy);
  void editProduct(const string& productID, uint category, const wstring& name, const wstring& description,
                   ullong price, uint stock, const string& imageDir);
  void updateSearchingData(const string& productID);
  Product* getProduct(const string& productID);
  list<Product*> getProductsByCategory(const string& category);
  list<Product*> getProductsByInput(wstring& input);
  list<Product*> getAllProducts();
  list<Product*> getAllStaffProducts();
};
