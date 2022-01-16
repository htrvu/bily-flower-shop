#include "ProductProvider.h"

#include "../AccountProvider/AccountProvider.h"
#include "../OrderProvider/OrderProvider.h"
#include "../VoucherProvider/VoucherProvider.h"

#include "../../Components/Account/Seller.h"
#include "../../Components/Account/Buyer.h"

///////////////////////////////////////////////////////////////////////////////////
// Constructor and Destructor

ProductProvider::ProductProvider() {
  readData();
}

ProductProvider* ProductProvider::getInstance() {
  static ProductProvider instance;
  return &instance;
}

ProductProvider::~ProductProvider() {
  writeData();

  for (auto i = _products.begin(); i != _products.end(); i++) {
    delete i->second;
  }
}

///////////////////////////////////////////////////////////////////////////////////
// Manipulation methods

// Read the information of products from "database"
void ProductProvider::readData() {
  json jsonData = readJSON(PRODUCTS_PATH);

  _counter = jsonData["COUNTER"];

  vector<string> temp = jsonData["NAMES"];
  _categoryNames = temp;

  for (auto i = jsonData["INFOS"].begin(); i != jsonData["INFOS"].end(); i++) {
    Product* product = nullptr;
    json info = i.value();
    string ID = i.key();

    wstring name = utf16(info["name"]);
    uint category = info["category"];
    string sellerUsername = info["sellerUsername"];
    wstring description = utf16(info["description"]);
    ullong originalPrice = info["originalPrice"];
    ullong salePrice = info["salePrice"];
    uint stock = info["stock"];
    uint sold = info["sold"];
    vector<uint> ratings = info["ratings"];
    string imageDir = info["imageDir"];
    vector<pair<float, int>> temp = info["discounts"];
    multiset<pair<float, int>> discounts(temp.begin(), temp.end());

    product = new Product(ID, category, name, sellerUsername, description, originalPrice, salePrice,
                          discounts, stock, sold, ratings, imageDir);

    _products[ID] = product;
  }

  for (auto i = jsonData["CATEGORIES"].begin(); i != jsonData["CATEGORIES"].end(); i++) {
    string category = i.key();
    list<string> productIDs = i.value();

    _categories[category] = productIDs;
  }

  // Read the data for searching
  ifstream f(SEARCH_RESOURCES);
  string ID, data;
  while (1) {
    getline(f, ID);
    getline(f, data);

    if (f.eof()) {
      break;
    }

    _searchingData[ID] = data;
  }
  f.close();
}

// Write the information of products to "database"
void ProductProvider::writeData() {
  json jsonData;

  jsonData["COUNTER"] = _counter;
  jsonData["NAMES"] = _categoryNames;

  for (auto i = _products.begin(); i != _products.end(); i++) {
    string ID = i->first;
    json info = i->second->getFullInfor();
    jsonData["INFOS"][ID] = info;
  }

  jsonData["CATEGORIES"]["birthday"] = _categories["birthday"];
  jsonData["CATEGORIES"]["congrats"] = _categories["congrats"];
  jsonData["CATEGORIES"]["love"] = _categories["love"];
  jsonData["CATEGORIES"]["office"] = _categories["office"];
  jsonData["CATEGORIES"]["sad"] = _categories["sad"];

  writeJSON(jsonData, PRODUCTS_PATH);

  // Write the searching data
  ofstream f(SEARCH_RESOURCES);
  for (auto i = _searchingData.begin(); i != _searchingData.end(); i++) {
    f << i->first << endl;
    f << i->second << endl;
  }
  f.close();
}

// Add product to "database"
void ProductProvider::addProduct(Product* product) {
  string ID = nextID();
  product->setID(ID);

  Seller* seller = (Seller*)AccountProvider::getInstance()->getAccount(product->sellerUsername());
  seller->addProduct(ID);

  _products[ID] = product;

  string category = _categoryNames[product->category()];
  _categories[category].push_back(ID);

  // Add the main content of this product to searching data
  wstring data = product->name() + utf16(" ") + product->description();
  _searchingData[product->ID()] = getMainContent(data);
}

// Delete product from "database"
bool ProductProvider::removeProduct(const string& productID) {
  if (_products.find(productID) == _products.end()) {
    return false;
  }

  // Remove this product from buyer carts
  AccountProvider* accountProvider = AccountProvider::getInstance();
  list<Account*> buyerIDs = accountProvider->getAccounts("buyers");

  for (auto i = buyerIDs.begin(); i != buyerIDs.end(); i++) {
    Buyer* buyer = (Buyer*)(*i);
    buyer->cart()->removeItem(productID);
  }

  // Remove orders which are related to this product
  OrderProvider* orderProvider = OrderProvider::getInstance();
  orderProvider->removeOrdersByProduct(productID);

  // Remove this product from related vouchers
  VoucherProvider* voucherProvider = VoucherProvider::getInstance();
  voucherProvider->removeProductFromVouchers(productID);

  // Remove this product from seller products
  Product* product = _products[productID];
  Seller* seller = (Seller*)AccountProvider::getInstance()->getAccount(product->sellerUsername());
  seller->removeProduct(productID);

  // Remove this product from categories
  string category = getCategoryName(product->category());
  _categories[category].remove(productID);

  // Remove this product from searching data
  _searchingData.erase(productID);

  delete _products[productID];
  _products.erase(productID);

  return true;
}

void ProductProvider::changeCategory(const string& productID, uint oldCategory, uint newCategory) {
  if (oldCategory == newCategory) {
    return;
  }

  string category = getCategoryName(oldCategory);
  _categories[category].remove(productID);

  category = getCategoryName(newCategory);
  _categories[category].push_back(productID);
}

// Edit the information of a Product
void ProductProvider::editProduct(const string& productID, uint category, const wstring& name,
                                  const wstring& description, ullong price, uint stock, const string& imageDir) {
  
  Product* product = _products[productID];

  // Change the _categories
  this->changeCategory(productID, product->category(), category);
  
  Product* newInforProduct = new Product(category, name, product->sellerUsername(), description, price, stock, imageDir);
  product->editInfor(newInforProduct);
  delete newInforProduct;

  // Update the searching data
  this->updateSearchingData(productID);
}

// Change the main content of the product (when its information is edited)
void ProductProvider::updateSearchingData(const string& productID) {
  Product* product = _products[productID];

  wstring data = product->name() + utf16(" ") + product->description();
  _searchingData[productID] = getMainContent(data);
}

// Get the category by an index (from 0)
string ProductProvider::getCategoryName(uint index) {
  return _categoryNames[index];
}

// Get the ID of next product
string ProductProvider::nextID() {
  _counter++;
  return "P" + to_string(_counter);
}

// Get the pointer to product by ID
Product* ProductProvider::getProduct(const string& productID) {
  if (_products.find(productID) != _products.end()) {
    return _products[productID];
  }

  return nullptr;
}

// Get the product ID list in a category
list<Product*> ProductProvider::getProductsByCategory(const string& category) {
  list<Product*> products;

  for (auto i = _categories[category].begin(); i != _categories[category].end(); i++) {
    products.push_back(_products[*i]);
  }

  return products;
}

// Get the product ID list related to a searching input

//Search Product By Key
list<Product*> ProductProvider::getProductsByInput(wstring& input) {
  string data = getMainContent(input);

  // Tokenize the data
  vector<string> words = getTokens(data, " ");

  // Find the related products
  // Store the result in a multiset<int, string>, in which:
  // int: score, string: productID
  multiset<pair<int, string>> productScores;
  for (auto i = _searchingData.begin(); i != _searchingData.end(); i++) {
    int score = 0;
    data = i->second;

    for (int j = 0; j < words.size(); j++) {
      int startPos = 0;
      int pos = data.find(words[j], startPos);
      while (pos >= 0) {
        score++;
        startPos = pos + 1;
        pos = data.find(words[j], startPos);
      }
    }

    if (score > 0) {
      productScores.insert({score, i->first});
    }
  }

  //Get List Product
  list<Product*> relatedProducts;
  for (auto i = productScores.begin(); i != productScores.end(); i++) {
    relatedProducts.push_front(_products[i->second]);
  }

  return relatedProducts;
}

// Get all of the products in the shop
list<Product*> ProductProvider::getAllProducts() {
  list<Product*> products;

  for (auto i = _products.begin(); i != _products.end(); i++) {
    products.push_back(i->second);
  }

  return products;
}

// Get all of the products posted by Staffs
list<Product*> ProductProvider::getAllStaffProducts() {
  list<Account*> staffs = AccountProvider::getInstance()->getAccounts(STAFF);

  list<Product*> products;
  for (auto i = staffs.begin(); i != staffs.end(); i++) {
    Seller* seller = (Seller*)(*i);
    list<string> productIDs = seller->products();

    for (auto j = productIDs.begin(); j != productIDs.end(); j++) {
      products.push_back(_products[*j]);
    }
  }

  return products;
}
