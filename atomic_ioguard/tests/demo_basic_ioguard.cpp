/// \file demo_basic_ioguard.cpp
/// \brief This is a simple  demonstration of atomic_ioguard. It creates a single record, and reads the same record.
#include "atomic_ioguard.h"
#include <iostream>
#include <assert.h>

using namespace util_ipc;
using namespace std;

int main() {
  uint64_t i = 0x12345678, j;
  atomic_ioguard<uint64_t> x(i), y, z;
#if 0
  assert( i == x.get_data());

  y.init_reverse_order( &x);
  assert( i == y.get_data());

  x.copy_to_rhs_reverse_order( &j);
  assert( i==j);

  cout << "test passed" << endl;
#endif
}
