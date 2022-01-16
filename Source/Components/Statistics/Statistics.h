#pragma once

#include "../../CONSTANTS.h"
#include "../Product/Product.h"

#include <list>
using namespace std;

class Statistics {
public:
  ullong totalRevenue;
  double avgRating;
  uint soldQuantity;
  uint shopQuantity;
  ullong shopRevenue;
  long long profitFromSeller;
  list<Product*> hitProducts;
  list<Product*> staleProducts;
};
