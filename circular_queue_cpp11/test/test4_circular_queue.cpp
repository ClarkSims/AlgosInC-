/// \file test4_circular_queue.cpp
/// \brief  This program measure the transfer rate of data between parent and child process.
#include "circular_queue.h"
#include "CustomRand.h"

#include <iostream>
#include <boost/date_time.hpp>

using namespace util_ipc;
using namespace std;

#define SIZE   1024
#define N      16777216
#define OFFSET 5

int main() 
{
  int32_t sz = sizeof( circular_queue<int,SIZE>);
  void *blob = new char[sz];
  circular_queue<int,SIZE> *cq = circular_queue<int,SIZE>::factory( blob, "foo");

  ax_b_c_random Rand;

  int i, j;
  for (i=0; i<OFFSET; ++i) {
    *cq->end() = i;
    cq->push_back();
  }

  int k, l;

  boost::posix_time::ptime t0 = boost::posix_time::microsec_clock::universal_time();
  boost::posix_time::ptime t1;

  for (j=0; i<N; ) {

    Rand.put_seed( i + j);
    k = Rand.rand() % SIZE + 1;
    
    for (l=0;  l<k; ++l) {

      if (cq->begin() == 0) break; // buffer empty

      if (j != *cq->begin()) {
	cerr << "test failed, j = " << j << " *head = " << *cq->begin() << endl;
	return -1;
      }
    
      cq->pop_front();
      ++j;
    }

    Rand.put_seed( i + j);
    k = Rand.rand() % SIZE + 1;

    for (l=0; l<k && i<N; ++l) {

      if (cq->end() == 0) break;  // buffer full

      *cq->end() = i;
      cq->push_back();
      ++i;
    }
  }

  t1 = boost::posix_time::microsec_clock::universal_time();

  boost::posix_time::time_duration dt = t1 - t0;

  cout << "test passed!" << endl;

  double tm = dt.total_microseconds();
  tm /= N * sizeof(int);

  cout << "micro seconds / byte = " << tm << endl;

  return 0;
}
