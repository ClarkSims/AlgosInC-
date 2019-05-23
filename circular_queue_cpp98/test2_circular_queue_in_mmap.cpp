/// \file test2_circular_queue_in_mmap.cpp
/// \brief This program is used to create a partially full circular queu in memory, and then read it.
/// It demonstrates that the shared memory is persistant, ie.it continues to work even after the
/// end of the process which created it.
#include "circular_queue_in_mmap.h"
#include "precision_clock.h"
#include <iostream>
#include <stdlib.h>

using namespace util_ipc;
using namespace std;

#define FNAME "test2_cqsm"
#define SIZE   1024
#define N      ((SIZE*2)/3)

void tolower( string &s) {
  size_t i;
  for(i=0; i<s.size(); ++i) {
    s[i] = (char)tolower(s[i]);
  }
}

int main( int argc, char *argv[]) 
{

  if (argc != 2) {
    cerr << "usage  test2_circular_queue_in_mmap  [parent|step_parent|child|cleanup]" << endl;
    return 1;
  }

  string argv1( argv[1]);

  tolower( argv1);

  using namespace boost::interprocess;

  if (argv1 == "parent" || argv1=="step_parent") {  //Parent process
    circular_queue_in_mmap<int64_t,SIZE> *cq;

    if (argv1 == "parent") {
      //cq = new circular_queue_in_mmap<int64_t,SIZE>( test2_cqsm, util_ipc::_create_only_);
      cq = circular_queue_in_mmap<int64_t,SIZE>::create_only( "test2_cqsm");
    } else {
      //cq = new circular_queue_in_mmap<int64_t,SIZE>( "test2_cqsm", util_ipc::_open_only_);
      cq = circular_queue_in_mmap<int64_t,SIZE>::open_only( "test2_cqsm");
    }

    cout << "number data in queue = "
         << cq->get_number_pushed() - cq->get_number_popped()
         << endl;

    int i;
    for (i=0; i<N;) {
      if (cq->end() != 0) {
	*cq->end() = i;
	cq->push_back();
	++i;
      }
    }

    std::cout << "test passed in parent process" << std::endl;

    delete cq;

    return 0;
  } else  if (argv1 == "child") {
    //circular_queue_in_mmap<int64_t,SIZE> *cq =
    //new circular_queue_in_mmap<int64_t,SIZE>( "test2_cqsm", util_ipc::_open_only_);
    circular_queue_in_mmap<int64_t,SIZE> *cq =   
      circular_queue_in_mmap<int64_t,SIZE>::open_only( "test2_cqsm");
 
    int i;

    precision_clock pc;
    uint64_t t0 = pc.nanoseconds_since_midnight(), t1;

    for (i=0; i<N; ) {
      if (cq->begin() != 0) {
	if (i != *cq->begin()) {
	  cerr << "test failed at offset i = " << i << " *cq->begin = " << *cq->begin() << endl;
	  return 1;
	}
	cq->pop_front();
	++i;
      }
    }

    t1 = pc.nanoseconds_since_midnight();

    uint64_t dt = t1 - t0;

    cerr << "test passed in Child process!" << endl;

    double tm = (double)dt / (N * sizeof(int64_t));

    cout << "nano seconds / byte = " << tm << endl;

    return 0;
  } else if (argv1=="cleanup") {

    //boost::interprocess::mmap_object::remove( ""test2_cqsm"");
  } else {
    cerr << "argv[1] = {" << argv1 << "} is not recognized, it should be parent or child" << endl;
    return 2;
  }
}
