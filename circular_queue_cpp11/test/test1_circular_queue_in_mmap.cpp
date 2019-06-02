#include "circular_queue_in_mmap.h"

#include <iostream>

using namespace util_ipc;
using namespace std;

#define SIZE   20
#define N      100
#define OFFSET 5

int main() {
  int i, j;

  {
    circular_queue_in_mmap<int,SIZE> *cq = 
      new circular_queue_in_mmap<int,SIZE>( "circular_queue.tmp", _create_only_); 

    for (i=0; i<OFFSET; ++i) {
      *cq->end() = i;
      cq->push_back();
    }

    delete cq;
  }

  {
    circular_queue_in_mmap<int,SIZE> *cq = 
      new circular_queue_in_mmap<int,SIZE>( "circular_queue.tmp", _open_only_); 

    for (j=0; i<N; ++i, ++j) {
      if (j != *cq->begin()) {
        cerr << "test failed, j = " << j << " *head = " << *cq->begin() << endl;
        return -1;
      }

      cq->pop_front();

      *cq->end() = i;
      cq->push_back();
    }
    delete cq;
  }

  cout << "test passed!" << endl;


  return 0;
}
