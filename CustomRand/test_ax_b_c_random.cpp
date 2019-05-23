#include <iostream>

#include "CustomRand.h"

using namespace std;

int main() {
  ax_b_c_random  rnd;

  for (int i=0;  i<100; i++) {
    cout << i << " " << rnd.rand() << endl;
  }

  return 0;
}
