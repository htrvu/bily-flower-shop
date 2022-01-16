#pragma once

#include "../../CONSTANTS.h"
#include "../../LIBRARIES.h"

#include "../../Components/Voucher/Voucher.h"

#include <unordered_map>
#include <list>

class VoucherProvider {
private:
  unordered_map<string, Voucher*> _vouchers;
  list<string> _events;
  uint _counter;

private:
  VoucherProvider();
  VoucherProvider(const VoucherProvider&) {}
  VoucherProvider& operator=(const VoucherProvider&) { return *this; }

public:
  static VoucherProvider* getInstance();
  ~VoucherProvider();

public:
  void readData();
  void writeData();

private:
  string nextID();
public:
  Voucher* latestEvent();
  list<string> events();
  Voucher* getVoucher(const string& VoucherID);
  void updatePricesWithVoucher(Voucher* voucher, bool isCreated);
  void removeProductFromVouchers(const string& productID);
  void addVoucher(Voucher* voucher);
  bool removeVoucher(string VoucherID);
  void editVoucher(const string& voucherID, const wstring& name, double salePercent, list<string> productIDs = {});
};
