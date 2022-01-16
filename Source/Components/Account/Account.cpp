#include "Account.h"

////////////////////////////////////////////////////////////////
// Constructors and Destructor

Account::Account() {
  _username = "";
  _name = L"";
  _phoneNumber = "";
  _address = L"";
  _role = "";
  _wallet = new Wallet;
}

Account::Account(string username, wstring name, string phoneNumber, wstring address, ullong balance, string role) {
  _username = username;
  _name = name;
  _phoneNumber = phoneNumber;
  _address = address;
  _role = role;
  _wallet = new Wallet(balance);
}

Account::Account(const Account& other) {
  _username = other._username;
  _name = other._name;
  _phoneNumber = other._phoneNumber;
  _address = other._address;
  _role = other._role;
  _wallet = new Wallet(*other._wallet);
}

#include <iostream>
Account::~Account() {
  delete _wallet;
  _wallet = nullptr;
}

////////////////////////////////////////////////////////////////
// Operators

// Asignment operator
Account& Account::operator=(const Account& other) {
  _username = other._username;
  _name = other._name;
  _phoneNumber = other._phoneNumber;
  _address = other._address;
  _role = other._role;
  _wallet = new Wallet(*other._wallet);

  return *this;
}

////////////////////////////////////////////////////////////////
// Getters and Setters

string Account::username() const {
  return _username;
}

wstring Account::name() const {
  return _name;
}

string Account::phoneNumber() const {
  return _phoneNumber;
}

wstring Account::address() const {
  return _address;
}

ullong Account::balance() const {
  return _wallet->balance();
}

string Account::role() const {
  return _role;
}

Wallet* Account::wallet() {
  return _wallet;
}

void Account::setRole(string role) {
  _role = role;
}

json Account::getGeneralInfor() const {
  json infor;
  infor["name"] = utf8(_name);
  infor["phoneNumber"] = _phoneNumber;
  infor["address"] = utf8(_address);
  infor["role"] = _role;
  infor["balance"] = this->balance();

  return infor;
}

////////////////////////////////////////////////////////////////
// Manipulation methods

void Account::editInfo(wstring name, string phoneNumber, wstring address) {
  _name = name;
  _phoneNumber = phoneNumber;
  _address = address;
}