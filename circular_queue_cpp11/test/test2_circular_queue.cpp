/// \file test2_circular_queue.cpp
/// \brief This program does a simple test to where data is put into a queue, and popped out. It is tested
///  to make sure the data coming out, is what it is supposed to be. This test is more strenuous than
///  test1_circular_queue.cpp,  because the data is from a pseudo random number generator.
#include "circular_queue.h"
#include "CustomRand.h"

#include <iostream>

using namespace util_ipc;
using namespace std;

#define SIZE   1024
#define N      1048576
#define OFFSET 5

int main() 
{
  int32_t sz = sizeof( circular_queue<int,SIZE>);
  void *blob = new char[sz];
  circular_queue<int,SIZE> *cq = circular_queue<int,SIZE>::factory( blob, "foo");

  ax_b_c_random Rand;

  int i, j, J;
  unsigned int I;
  for (i=0; i<OFFSET; ++i) {
    Rand.put_seed( i);
    I = Rand.rand();
    *cq->end() = I;
    cq->push_back();
  }

  for (j=0; i<N; ++i, ++j) {

    Rand.put_seed( j);
    J = Rand.rand();

    if (J != *cq->begin()) {
      cerr << "test failed, j = " << j << " *head = " << *cq->begin() << endl;
      return -1;
    }
    
    cq->pop_front();

    Rand.put_seed( i);
    I = Rand.rand();

    if (cq->end() == 0) {
      cerr << "error , buffer is full" << endl;
      return 1;
    }
    *cq->end() = I;
    cq->push_back();
  }

  cout << "test passed!" << endl;
  return 0;
}
