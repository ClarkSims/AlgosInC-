/// \file test1_circular_queue.cpp
/// \brief This program does a simple test to where data is put into a queue, and popped out. It is tested
///  to make sure the data coming out, is what it is supposed to be.
#include "circular_queue.h"

#include <iostream>
#include <vector>

using namespace util_ipc;
using namespace std;

// SIZE is 2 pages
#define SIZE   8192
#define N      100
#define OFFSET 5

void test1() {
  int32_t sz = sizeof( circular_queue<int,SIZE>);
  std::vector<char> blob_alloc(sz, 'z');
  char *blob = &*blob_alloc.begin();
  circular_queue<unsigned int,SIZE> *cq = circular_queue<unsigned int,SIZE>::factory( blob);

  unsigned int i, j, k;
  for (i=0; i<OFFSET; ++i) {
    *cq->end() = i;
    cq->push_back();
  }

  for (j=0; i<N; ++i, ++j) {
    if (j != *cq->begin()) {
      cerr << "test failed, j = " << j << " *head = " << *cq->begin() << endl;
      exit( 1);
    }
    for (k=j; k<(j+cq->size());++k) {
      if (k != cq->at(k)) {
        cerr << "test failed, j = " << j << " k = " << k << " cq->at(k) = " << cq->at(k) << endl;
        exit( 2);
      }
    }

    cq->pop_front();

    *cq->end() = i;
    cq->push_back();
  }
}

void test2() {
  int32_t sz = sizeof( circular_queue<int,SIZE>);
  std::vector<char> blob_alloc(sz, 'z');
  char *blob = &*blob_alloc.begin();
  circular_queue<unsigned int,SIZE> *cq = circular_queue<unsigned int,SIZE>::factory( blob);

  unsigned int i, j, k;
  for (i=0; i<OFFSET; ++i) {
    *cq->atomic_push_back_get_end() = i;
  }

  for (j=0; i<N; ++i, ++j) {
    if (j != *cq->begin()) {
      cerr << "test failed, j = " << j << " *head = " << *cq->begin() << endl;
      exit( 1);
    }
    for (k=j; k<(j+cq->size());++k) {
      if (k != cq->at(k)) {
        cerr << "test failed, j = " << j << " k = " << k << " cq->at(k) = " << cq->at(k) << endl;
        exit( 2);
      }
    }

    cq->pop_front();

    *cq->atomic_push_back_get_end() = i;
  }
}


int main() 
{
  test1();
  test2();

  cout << "test passed!" << endl;

  return 0;
}
