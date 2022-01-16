#include "AccountProvider.h"
#include "../../Components/Account/Buyer.h"
#include "../../Components/Account/Seller.h"
#include "../../Components/Account/Staff.h"
#include "../../Components/Account/Admin.h"

#include "../../Providers/ProductProvider/ProductProvider.h"
#include "../../Providers/VoucherProvider/VoucherProvider.h"
#include "../../Providers/LoginProvider/LoginProvider.h"

///////////////////////////////////////////////////////////////////////////////////////
// Constructor and Destructor

AccountProvider::AccountProvider() {
  readData();
}

// Get the instance of AccountProvider (Singleton)
AccountProvider* AccountProvider::getInstance() {
  static AccountProvider instance;
  return &instance;
}

AccountProvider::~AccountProvider() {
  writeData();

  Admin* admin = Admin::getInstance();

  for (auto i = _accounts.begin(); i != _accounts.end(); i++) {
    if (i->first != admin->username()) {
      delete i->second;
      i->second = nullptr;
    }
  }
}

///////////////////////////////////////////////////////////////////////////////////////
// Manipulation methods

// Read the information and data of all accounts (sellers and buyers) from "database"
void AccountProvider::readData() {
  json jsonData = readJSON(ACCOUNTS_PATH);

  // Username list
  for (auto i = jsonData["USERS"].begin(); i != jsonData["USERS"].end(); i++) {
    string key = i.key();
    list<string> usernames = i.value();
    _usernameList[key] = usernames;
  }

  // Creating all accounts base on general information
  for (auto i = jsonData["INFOS"].begin(); i != jsonData["INFOS"].end(); i++) {
    Account* account = nullptr;
    json info = i.value();

    // -------------------------------------------------------------------------
    // General information
    string username = i.key();
    wstring name = utf16(info["name"]);
    string phoneNumber = info["phoneNumber"];
    wstring address = utf16(info["address"]);
    ullong balance = info["balance"];
    string role = info["role"];

    if (role == SELLER) {
      account = new Seller(username, name, phoneNumber, address, balance);
    } else if (role == BUYER) {
      account = new Buyer(username, name, phoneNumber, address, balance);
    } else if (role == STAFF) {
      account = new Staff(username, name, phoneNumber, address, balance);
    } else {
      account = Admin::getInstance(username, name, phoneNumber, address, balance);
    }

    _accounts[username] = account;
  }

  // -------------------------------------------------------------------------
  // Own information for each type of account
  vector<string> types = {"SELLERS", "STAFFS", "BUYERS", "ADMINS"};
  for (auto type : types) {
    for (auto i = jsonData[type].begin(); i != jsonData[type].end(); i++) {
      string username = i.key();
      json data = i.value();
      _accounts[username]->setOwnInfor(data);
    }
  }
}

// Write the information and data of all accounts (sellers and buyers) to database
void AccountProvider::writeData() {
  json jsonData;

  // Username list
  jsonData["USERS"]["buyers"] = _usernameList["buyers"];
  jsonData["USERS"]["sellers"] = _usernameList["sellers"];
  jsonData["USERS"]["staffs"] = _usernameList["staffs"];

  // Informations of all accounts
  for (auto i = _accounts.begin(); i != _accounts.end(); i++) {
    string username = i->first;

    // -------------------------------------------------------------------------
    // Write the general information
    json infor = i->second->getGeneralInfor();
    jsonData["INFOS"][username] = infor;

    // -------------------------------------------------------------------------
    // Write the own information of each account type
    string role = i->second->role();
    json ownInfor = i->second->getOwnInfor();

    // Write for both sellerss
    if (role == SELLER) {
      jsonData["SELLERS"][username] = ownInfor;
    }
    // Write for buyers
    else if (role == BUYER) {
      jsonData["BUYERS"][username] = ownInfor;
    }
    // Write for staff
    else if (role == STAFF) {
      jsonData["STAFFS"][username] = ownInfor;
    }
    // Write for admin
    else {
      jsonData["ADMINS"][username] = ownInfor;
    }
  }

  // Write all of the data of to json file
  writeJSON(jsonData, ACCOUNTS_PATH);
}

// Get account by username
Account* AccountProvider::getAccount(const string& username) {
  if (_accounts.find(username) == _accounts.end()) {
    return nullptr;
  }

  return _accounts[username];
}

// Get all the accounts of a role
list<Account*> AccountProvider::getAccounts(const string& role) {
  list<Account*> accounts;

  string roleMap = "";
  if (role == BUYER) {
    roleMap = "buyers";
  } else if (role == SELLER) {
    roleMap = "sellers";
  } else if (role == STAFF) {
    roleMap = "staffs";
  }

  for (auto i = _usernameList[roleMap].begin(); i != _usernameList[roleMap].end(); i++) {
    string username = *i;
    accounts.push_back(_accounts[username]);
  }

  return accounts;
}


// Add account to database
void AccountProvider::addAccount(Account* account) {
  _accounts[account->username()] = account;

  if (account->role() == BUYER) {
    _usernameList["buyers"].push_back(account->username());
  } else if (account->role() == SELLER) {
    _usernameList["sellers"].push_back(account->username());
  } else if (account->role() == STAFF) {
    _usernameList["staffs"].push_back(account->username());
  }
}

// Remove account from database
void AccountProvider::removeAccount(const string& username, bool temporary) {
  Account* account = getAccount(username);
  string role = account->role();

  // Remove from userList
  if (role == BUYER) {
    _usernameList["buyers"].remove(account->username());
  } else if (role == SELLER) {
    _usernameList["sellers"].remove(account->username());
  } else if (role == STAFF) {
    _usernameList["staffs"].remove(account->username());
  }

  // If this account is Seller or Staff
  if (role == SELLER || role == STAFF) {
    VoucherProvider* voucherProvider = VoucherProvider::getInstance();

    // If this is a "real" remove, remove all the product this account posted
    // By this operation, the order and voucher related to this account will be also removed
    if (temporary == false) {
      ProductProvider* productProvider = ProductProvider::getInstance();

      list<string> productIDs = ((Seller*)account)->products();
      for (auto i = productIDs.begin(); i != productIDs.end(); i++) {
        productProvider->removeProduct(*i);
      }
      // Remove all events this staff created
      if (role == STAFF) {

        list<string> vouchers =((Seller*)account)->vouchers();
        for (auto i = vouchers.begin(); i != vouchers.end(); i++) {
            voucherProvider->removeVoucher(*i);
        }
      }
    }
    // Otherwise, this is just a temporary remove for some other operations
    // For example: Admin adds a Staff, from a Seller
    // ---> Remove all vouchers (or events) this account created
    else {
        list<string> vouchers = ((Seller*)account)->vouchers();
        for (auto i = vouchers.begin(); i != vouchers.end(); i++) {
            voucherProvider->removeVoucher(*i);
        }
    }
  }

  delete _accounts[username];

  // If this is a "real" remove, remove the username and password in LoginProvider
  if (temporary == false) {
    LoginProvider::getInstance()->removeAccount(username);
    _accounts.erase(username);
  } else {
    _accounts[username] = nullptr;
  }
}

// Get the statistics of an account(Seller, Staff or Admin)
Statistics AccountProvider::getStatistics(Account* account) {
  string role = account->role();

  ullong totalRevenue = 0;
  double avgRating = 0;
  uint soldQuantity = 0;
  list<Product*> hitProducts;
  list<Product*> staleProducts;
  uint shopQuantity;

  // More information for Admin
  ullong shopRevenue;
  long long profitFromSeller;

  ProductProvider* productProvider = ProductProvider::getInstance();
  vector<Product*> products;

  // Get the statistics of a Seller (or Staff)
  if (role == SELLER || role == STAFF) {
    Seller* seller = (Seller*)account;

    list<string> temp = seller->products();

    if (!temp.empty()) {
      for (auto ID: temp) {
        products.push_back(productProvider->getProduct(ID));
      }
    }

    totalRevenue = seller->revenue();
    avgRating = seller->avgRating();
    soldQuantity = seller->soldQuantity();
    shopQuantity = temp.size();
  }
  // Get the statistics of Admin
  else if (role == ADMIN) {
    Admin* admin = (Admin*)account;

    list<Product*> temp = productProvider->getAllStaffProducts();

    if (!temp.empty()) {
      for (auto product: temp) {
        products.push_back(product);
        avgRating += product->avgRating();
        soldQuantity += product->sold();
      }
      avgRating /= temp.size();
    }

    shopRevenue = admin->shopRevenue();
    profitFromSeller = admin->profitFromSeller();
    totalRevenue = shopRevenue + profitFromSeller;
    shopQuantity = temp.size();
  }

  // Sort the product list by sold quantity
  sort(products.begin(), products.end(), [](Product* a, Product* b) {
    return a->sold() > b->sold();
  });

  int n = products.size();
  if (n != 0) {
    // Get the hit products and stale (top 5)
    if (n <= TOP_PRODUCT * 2) {
      for (int i = 0; i <= n / 2; i++) {
        hitProducts.push_back(products[i]);
      }
      for (int i = n / 2 + 1; i < n; i++) {
        staleProducts.push_back(products[i]);
      }
    }
    else {
      for (int i = 0; i < TOP_PRODUCT; i++) {
        hitProducts.push_back(products[i]);
        staleProducts.push_back(products[n - 1 - i]);
      }
    }
  }

  // return the result object
  return Statistics{totalRevenue, avgRating, soldQuantity, shopQuantity, shopRevenue,
                    profitFromSeller, hitProducts, staleProducts};
}
