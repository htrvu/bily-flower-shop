#pragma once

#include "../../LIBRARIES.h"
#include "../../CONSTANTS.h"

#include "../Wallet/Wallet.h"

class Account {
private:
  string _username;
  wstring _name;
  string _phoneNumber;
  wstring _address;
  string _role;
  Wallet* _wallet;

public:
  Account();
  Account(string username, wstring name, string phoneNumber, wstring address, ullong balance, string role);
  Account(const Account& other);
  virtual ~Account();

public:
  Account& operator=(const Account& other);

public:
  string username() const;
  wstring name() const;
  string phoneNumber() const;
  wstring address() const;
  ullong balance() const;
  string role() const;
  Wallet* wallet();
  json getGeneralInfor() const;
  virtual json getOwnInfor() const = 0;
  virtual void setOwnInfor(json data) = 0;
protected:
  void setRole(string role);
  
public:
  void editInfo(wstring name, string phoneNumber, wstring address);
};