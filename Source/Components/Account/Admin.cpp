#include "Admin.h"

#include "../../Providers/AccountProvider/AccountProvider.h"

#include "Staff.h"

////////////////////////////////////////////////////////////////
// Constructors and Destructors

Admin::Admin() : Account() {}

Admin::Admin(string username, wstring name, string phoneNumber, wstring address, ullong balance,
             ullong shopRevenue, long long profitFromSeller)
    : Account(username, name, phoneNumber, address, balance, ADMIN) {
  _shopRevenue = shopRevenue;
  _profitFromSeller = profitFromSeller;
}

Admin* Admin::getInstance(string username, wstring name, string phoneNumber, wstring address,
                          ullong balance, ullong shopRevenue, long long profitFromSeller) {
  static Admin instance(username, name, phoneNumber, address, balance, shopRevenue,
                        profitFromSeller);
  return &instance;
}

////////////////////////////////////////////////////////////////
// Getters and Setters

list<string> Admin::staffs() const {
  return _staffs;
}

ullong Admin::shopRevenue() const {
  return _shopRevenue;
}

long long Admin::profitFromSeller() const {
  return _profitFromSeller;
}

json Admin::getOwnInfor() const {
  json infor;

  infor["staffs"] = _staffs;
  infor["shopRevenue"] = _shopRevenue;
  infor["profitFromSeller"] = _profitFromSeller;

  return infor;
}

void Admin::setOwnInfor(json data) {
  list<string> staffs = data["staffs"];
  ullong shopRevenue = data["shopRevenue"];
  long long profitFromSeller = data["profitFromSeller"];

  _staffs = staffs;
  _shopRevenue = shopRevenue;
  _profitFromSeller = profitFromSeller;
}

////////////////////////////////////////////////////////////////
// Manipulation methods

ullong Admin::totalSalaries() {
  ullong total = 0;
  for (string username : _staffs) {
    Staff* staff = (Staff*)(AccountProvider::getInstance()->getAccount(username));
    total += staff->salary();
  }

  return total;
}

bool Admin::paySalaries() {
  ullong total = this->totalSalaries();

  if (total > wallet()->balance()) {
    return false;
  }

  wallet()->withdraw(total);
  for (string username : _staffs) {
    Staff* staff = (Staff*)(AccountProvider::getInstance()->getAccount(username));
    staff->wallet()->deposit(staff->salary());

    // Reset bonus money of Staff
    staff->resetBonus();
  }

  return true;
}

void Admin::addStaff(const string& username) {
  _staffs.push_back(username);

  AccountProvider* accountProvider = AccountProvider::getInstance();

  // Create new Staff with the information from Seller
  Seller* seller = (Seller*)(accountProvider->getAccount(username));
  Staff* newStaff = new Staff(seller);

  // Handle in AccountProvider
  accountProvider->removeAccount(username);
  accountProvider->addAccount(newStaff);
}

void Admin::dismissStaff(const string& username) {
  _staffs.remove(username);

  // Handle in AccountProvider
  AccountProvider::getInstance()->removeAccount(username, false);
}

void Admin::addShopRevenue(ullong amount) {
  _shopRevenue += amount;
  this->wallet()->deposit(amount);
}

void Admin::addProfitFromSeller(long long amount) {
  _profitFromSeller += amount;

  if (amount > 0) {
    this->wallet()->deposit(amount);
  } else {
    this->wallet()->withdraw(amount);
  }
}
