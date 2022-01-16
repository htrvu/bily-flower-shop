#include "LoginProvider.h"

////////////////////////////////////////////////////////////////
// Constructor ans Destructor

LoginProvider::LoginProvider() {
  readData();
}

LoginProvider::~LoginProvider() {
  writeData();
}

// Get instance of this class (Singleton)
LoginProvider* LoginProvider::getInstance() {
  static LoginProvider instance;
  return &instance;
}

////////////////////////////////////////////////////////////////
// Manipulation methods

// Read the usernames and hashed passwords from database
void LoginProvider::readData() {
  json jsonData = readJSON(PASSWORDS_PATH);

  for (auto i = jsonData["HASHES"].begin(); i != jsonData["HASHES"].end(); i++) {
    _passwordStore[i.key()] = i.value();
	}
}

// Write data to database
void LoginProvider::writeData() {
  json jsonData;

  jsonData["HASHES"] = _passwordStore;
  
  writeJSON(jsonData, PASSWORDS_PATH);
}

// Add new account to database
bool LoginProvider::addAccount(const string& username, const string& password) {
  if (_passwordStore.find(username) != _passwordStore.end()) {
    return false;
  }

  string hashed = sha256(password);
  _passwordStore[username] = hashed;
  return true;
}

// Remove account from database
bool LoginProvider::removeAccount(const string& username) {
  if (_passwordStore.find(username) == _passwordStore.end()) {
    return false;
  }

  _passwordStore.erase(username);
  return true;
}

// Change the password of an existed account
bool LoginProvider::changePassword(const string& username, const string& currentPassword, const string& newPassword) {
  if (_passwordStore.find(username) == _passwordStore.end()) {
    return false;
  }
  
  string hashed = sha256(currentPassword);
  if (hashed != _passwordStore[username]) {
    return false;
  }

  hashed = sha256(newPassword);
  _passwordStore[username] = hashed;
  return true;
}

// Recover the password of an existed account
bool LoginProvider::recoverPassword(const string& username) {
  if (_passwordStore.find(username) == _passwordStore.end()) {
    return false;
  }

  // Set new password as "123456" by default.
  string defaultPassword = "123456";
  _passwordStore[username] = sha256(defaultPassword);
  return true;
}

// Login validation
bool LoginProvider::login(const string& username, const string& password) {
  string hashed = sha256(password);
  if (_passwordStore.find(username) == _passwordStore.end() || _passwordStore[username] != hashed) {
    return false;
  }

  return true;
}
