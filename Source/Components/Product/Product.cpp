#include "Product.h"

////////////////////////////////////////////////////////////////////////////////
// Constructors

Product::Product() {
  _ID = "";
  _name = L"";
  _sellerUsername = "";
  _description = L"";
  _originalPrice = 0;
  _salePrice = 0;
  _stock = 0;
  _ratings = {0, 0, 0, 0, 0};
  _imageDir = "";
}

Product::Product(string id, uint category, wstring name, string sellerUsername, wstring description,
                 ullong originalPrice, ullong salePrice,
                 multiset<pair<float, int>> discounts, uint stock, uint sold,
                 vector<uint> ratings, string imageDir) {
  _ID = id;
  _category = category;
  _name = name;
  _sellerUsername = sellerUsername;
  _description = description;
  _originalPrice = originalPrice;
  _salePrice = salePrice;
  _discounts = discounts;
  _stock = stock;
  _sold = sold;
  _ratings = ratings;
  _imageDir = imageDir;
}

Product::Product(uint category, wstring name, string sellerUsername, wstring description,
                 ullong originalPrice, uint stock, string imageDir) {
  _ID = "";
  _category = category;
  _name = name;
  _sellerUsername = sellerUsername;
  _description = description;
  _originalPrice = originalPrice;
  _salePrice = originalPrice;
  _stock = stock;
  _sold = 0;
  _imageDir = imageDir;
}

////////////////////////////////////////////////////////////////////////////////
// Getters and Setters

string Product::ID() const {
  return _ID;
}

uint Product::category() const {
  return _category;
}

wstring Product::name() const {
  return _name;
}

string Product::sellerUsername() const {
  return _sellerUsername;
}

wstring Product::description() const {
  return _description;
}

ullong Product::originalPrice() const {
  return _originalPrice;
}

ullong Product::salePrice() const {
  return _salePrice;
}

uint Product::stock() const {
  return _stock;
}

uint Product::sold() const {
  return _sold;
}

string Product::imageDir() const {
  return _imageDir;
}

vector<uint> Product::ratings() const {
  return _ratings;
}

float Product::avgRating() const {
  float rating = 0;
  int count = 0;
  for (int i = 0; i < _ratings.size(); i++) {
      rating += 1.0 * (i + 1) * _ratings[i];
      count += _ratings[i];
  }
  
  if (count == 0) return 0;
  return rating / count;
}

multiset<pair<float, int>> Product::discounts() const {
  return _discounts;
}

json Product::getFullInfor() const {
  json infor;

  infor["name"] = utf8(_name);
  infor["category"] = _category;
  infor["sellerUsername"] = _sellerUsername;
  infor["description"] = utf8(_description);
  infor["originalPrice"] = _originalPrice;
  infor["salePrice"] = _salePrice;
  infor["stock"] = _stock;
  infor["sold"] = _sold;
  infor["ratings"] = _ratings;
  infor["imageDir"] = _imageDir;
  infor["discounts"] = _discounts;

  return infor;
}

////////////////////////////////////////////////////////////////////////////////
// Manipulation methods

void Product::setID(const string& ID) {
  _ID = ID;
}

void Product::editInfor(Product* newProductInfor) {
  _category = newProductInfor->category();
  _name = newProductInfor->name();
  _description = newProductInfor->description();
  _originalPrice = newProductInfor->originalPrice();
  _stock = newProductInfor->stock();
  _imageDir = newProductInfor->imageDir();

  float discount = getDiscount();
  _salePrice = (100.0 - discount) * _originalPrice / 100;
}

void Product::addRating(uint rating) {
  _ratings[rating]++;
}

float Product::getDiscount() const {
  return (!_discounts.empty()) ? _discounts.rbegin()->first : 0;
}

bool Product::isInEvent() const {
  return !_discounts.empty() && _discounts.rbegin()->second == 0;
}

// Add a discount to the discount list
// type: 0 is event, 1 is voucher
void Product::addDiscount(float discount, int type) {
  _discounts.insert({discount, type});

  _salePrice = (100.0 - _discounts.rbegin()->first) * _originalPrice / 100;
}

// Remove a discount from the discount list
// type: 0 is event, 1 is voucher
void Product::removeDiscount(float discount, int type) {
  _discounts.erase(_discounts.find({discount, type}));

  if (!_discounts.empty()) {
    _salePrice = (100.0 - _discounts.rbegin()->first) * _originalPrice / 100;
  } else {
    _salePrice = _originalPrice;
  }
}

void Product::changeSoldQuantity(int amount) {
  _sold += amount;
}

void Product::changeStockQuantity(int amount) {
  _stock += amount;
}

