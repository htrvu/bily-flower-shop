#pragma once

#include "../../LIBRARIES.h"
#include "../../CONSTANTS.h"

#include "../../Components/Account/Account.h"
#include "../../Components/Statistics/Statistics.h"

#include <unordered_map>
#include <list>

// Singleton design pattern

class AccountProvider {
private:
  unordered_map<string, Account*> _accounts;
  unordered_map<string, list<string>> _usernameList;

private:
  AccountProvider();
  AccountProvider(const AccountProvider&) {}
  AccountProvider& operator=(const AccountProvider&) { return *this; }

public:
  static AccountProvider* getInstance();
  ~AccountProvider();

public:
  void readData();
  void writeData();
  Account* getAccount(const string& username);
  list<Account*> getAccounts(const string& role);
  void addAccount(Account* account);
  void removeAccount(const string& username, bool temporary = true);
  Statistics getStatistics(Account* account);
};
