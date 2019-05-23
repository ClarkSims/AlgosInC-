/// \file test3_circular_queue.cpp
/// \brief This program test the major interface functions of circular_queue,  begin(), end(), push_back() and 
/// pop_front.  It test the boundary conditions where the buffer is full and is empty also.
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

  int k, l;

  for (j=0; i<N; ) {

    Rand.put_seed( i + j);
    k = Rand.rand() % SIZE  + 1;
    
    for (l=0;  l<k; ++l) {
      Rand.put_seed( j); 
      J = Rand.rand();

      if (cq->begin() == 0) break; // buffer empty

      if (J != *cq->begin()) {
	cerr << "test failed, j = " << j << " *head = " << *cq->begin() << endl;
	return -1;
      }
    
      cq->pop_front();
      ++j;
    }

    Rand.put_seed( i + j);
    k = Rand.rand() % SIZE + 1;

    for (l=0; l<k && i<N; ++l) {

      Rand.put_seed( i);
      I = Rand.rand();

      if (cq->end() == 0) break;  // buffer full

      *cq->end() = I;
      cq->push_back();
      ++i;
    }
  }

  cout << "test passed!" << endl;
  return 0;
}
