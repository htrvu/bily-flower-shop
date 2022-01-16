#pragma once

#include "../../LIBRARIES.h"
#include "../../CONSTANTS.h"

#include <unordered_map>

// Singleton design pattern

class LoginProvider {
private:
  unordered_map<string, string> _passwordStore;

private:
  LoginProvider();
  LoginProvider(const LoginProvider&) {}
  LoginProvider& operator=(const LoginProvider&) { return *this; }

public:
  static LoginProvider* getInstance();
  ~LoginProvider();
  
public:
  void readData();
  void writeData();
  bool addAccount(const string& username, const string& password);
  bool removeAccount(const string& username);
  bool changePassword(const string& username, const string& currentPassword, const string& newPassword);
  bool recoverPassword(const string& username);
  bool login(const string& username, const string& password);
};

// Login Service
