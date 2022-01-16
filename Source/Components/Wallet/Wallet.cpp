#include "Wallet.h"

////////////////////////////////////////////////////////////////
// Constructors

Wallet::Wallet() {
  _balance = 0;
}

Wallet::Wallet(ullong balance) {
  _balance = balance;
}

////////////////////////////////////////////////////////////////
// Manipulation methods

ullong Wallet::balance() const {
  return _balance;
}

void Wallet::deposit(ullong amount) {
  _balance += amount;
}

bool Wallet::withdraw(ullong amount) {
  if (_balance >= amount) {
    _balance -= amount;
    return true;
  }

  return false;
}

void Wallet::setBalance(ullong balance) {
  _balance = balance;
}