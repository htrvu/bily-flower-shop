#pragma once

#include "../../CONSTANTS.h"

class Wallet {
private:
  ullong _balance;

public:
  Wallet();
  Wallet(ullong _balance);

public:
  ullong balance() const;
  void deposit(ullong amount);
  bool withdraw(ullong amount);
  void setBalance(ullong amount);
};