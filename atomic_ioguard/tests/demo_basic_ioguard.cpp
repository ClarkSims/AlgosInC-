/// \file demo_basic_ioguard.cpp
/// \brief This is a simple  demonstration of IOGuard. It creates a single record, and reads the same record.
#include "basic_ioguard.h"
#include "IOGuard.h"
#include <iostream>
#include <assert.h>

using namespace util_ipc;
using namespace std;

int main() {
  uint64_t i = 0x12345678, j;
  IOGuard<uint64_t> x(i), y, z;

  assert( i == x.get_data());

  y.init_reverse_order( &x);
  assert( i == y.get_data());

  x.copy_to_rhs_reverse_order( &j);
  assert( i==j);

  cout << "test passed" << endl;
}
