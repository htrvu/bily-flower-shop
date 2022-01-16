#include "VoucherProvider.h"

#include "../ProductProvider/ProductProvider.h"
#include "../AccountProvider/AccountProvider.h"

#include "../../Components/Account/Seller.h"

////////////////////////////////////////////////////////////////
// Constructor and Destructor

VoucherProvider::VoucherProvider() {
  readData();
}

// Get the instance of VoucherProvider
VoucherProvider* VoucherProvider::getInstance() {
  static VoucherProvider instance;
  return &instance;
}

VoucherProvider::~VoucherProvider() {
  writeData();

  for (auto i = _vouchers.begin(); i != _vouchers.end(); i++) {
    delete i->second;
    i->second = nullptr;
  }
}

// Read the information of vouchers from "database"
void VoucherProvider::readData() {
  json jsonData = readJSON(VOUCHER_PATH);

  uint counter = jsonData["COUNTER"];
  _counter = counter;

  list<string> events = jsonData["EVENTS"];
  _events = events;

  for (auto i = jsonData["VOUCHERS"].begin(); i != jsonData["VOUCHERS"].end(); ++i) {
    Voucher* voucher = nullptr;
    string voucherID = i.key();
    json info = i.value();

    double salePercent = info["salePercent"];
    string sellerUsername = info["sellerUsername"];
    wstring name = utf16(info["name"]);
    list<string> productIDs = info["productIDs"];

    voucher = new Voucher(voucherID, name, salePercent, sellerUsername, productIDs);
    _vouchers[voucherID] = voucher;
  }

}

// Write the information of products to "database"
void VoucherProvider::writeData() {
  json data;

  data["COUNTER"] = _counter;
  data["EVENTS"] = _events;

  for (auto i : _vouchers) {
    string ID = i.first;
    json infor = i.second->getFullInfor();
    data["VOUCHERS"][ID] = infor;
  }

  writeJSON(data, VOUCHER_PATH);
}

// Update the price of products when a voucher is added or removed
void VoucherProvider::updatePricesWithVoucher(Voucher* voucher, bool isCreated) {
  ProductProvider* productProvider = ProductProvider::getInstance();

  // If this voucher is created by Staff, which means that's an event, we will update the salePrice
  // of all products in the shop
  if (voucher->isEvent()) {
    list<Product*> products = productProvider->getAllProducts();

    for (Product* product: products) {
      if (isCreated) {
        product->addDiscount(voucher->salePercent(), 0);
      } else {
        product->removeDiscount(voucher->salePercent(), 0);
      }
    }

    return;
  }

  // Ortherwise, this is a normal voucher created by a Seller. We will update the salePrice of some products.
  list<string> productIDs;
  if (voucher->productIDs().front() == "all") {
    Seller* seller = (Seller*)AccountProvider::getInstance()->getAccount(voucher->sellerUsername());
    productIDs = seller->products();
  } else {
    productIDs = voucher->productIDs();
  }

  for (string productID : productIDs) {
    Product* product = productProvider->getProduct(productID);
    if (isCreated) {
      product->addDiscount(voucher->salePercent(), 1);
    } else {
      product->removeDiscount(voucher->salePercent(), 1);
    }
  }
}

void VoucherProvider::removeProductFromVouchers(const string &productID) {
  vector<Voucher*> emptyVouchers;

  for (auto i = _vouchers.begin(); i != _vouchers.end(); i++) {
    Voucher* voucher = i->second;
    bool notEmpty = voucher->removeProduct(productID);
    if (notEmpty == false) {
      emptyVouchers.push_back(voucher);
    }
  }

  // This vouchers have an empty productIDs, so they will be removed
  for (Voucher* voucher: emptyVouchers) {
    Seller* seller = (Seller*)(AccountProvider::getInstance()->getAccount(voucher->sellerUsername()));
    seller->removeVoucher(voucher->ID());
    _vouchers.erase(voucher->ID());
    delete voucher;
  }
}

// Generate the ID for next voucher
string VoucherProvider::nextID() {
  _counter++;
  string ID = "V" + to_string(_counter);

  return ID;
}

// Get the latest event of the shop
Voucher* VoucherProvider::latestEvent() {
  if (_events.empty()) {
    return nullptr;
  }

  return _vouchers[_events.back()];
}

list<string> VoucherProvider::events() {
    return _events;
}

// Get the voucher by ID
Voucher* VoucherProvider::getVoucher(const string& VoucherID) {
  if (_vouchers.find(VoucherID) == _vouchers.end())
    return nullptr;

  return _vouchers[VoucherID];
}

// Add a voucher to the "database"
void VoucherProvider::addVoucher(Voucher* voucher) {
  voucher->setID(nextID());

  _vouchers[voucher->ID()] = voucher;
  if (voucher->isEvent()) {
    _events.push_back(voucher->ID());
  }

  updatePricesWithVoucher(voucher, true);

  Seller* seller = (Seller*)(AccountProvider::getInstance()->getAccount(voucher->sellerUsername()));
  seller->addVoucher(voucher->ID());
}

// Remove a voucher from the "database"
bool VoucherProvider::removeVoucher(string voucherID) {
  if (_vouchers.find(voucherID) == _vouchers.end()) {
    return false;
  }

  Voucher* voucher = _vouchers[voucherID];

  updatePricesWithVoucher(voucher, false);

  Seller* seller = (Seller*)(AccountProvider::getInstance()->getAccount(voucher->sellerUsername()));
  seller->removeVoucher(voucherID);

  if (voucher->isEvent()) {
    _events.remove(voucherID);
  }

  delete voucher;
  _vouchers.erase(voucherID);
  return true;
}

// Edit the information of a voucher
void VoucherProvider::editVoucher(const string& voucherID, const wstring& name, double salePercent, list<string> productIDs) {
  Voucher* voucher = _vouchers[voucherID];
  
  // Cancel the effect of old Voucher
  this->updatePricesWithVoucher(voucher, false);

  Voucher* newVoucherInfor = new Voucher("", name, salePercent, voucher->sellerUsername(), productIDs);
  voucher->editInfor(newVoucherInfor);
  delete newVoucherInfor;

  // Update the effect of new Voucher
  this->updatePricesWithVoucher(voucher, true);
}
