/// \file test1_circular_queue_in_shared_memory.cpp
///  \brief This program measures the transfer rate between two processes communicating data with a circular queue
///  in shared memory. It uses the class circular_queue_in_shared_memory.
#include "circular_queue_in_shared_memory.h"

#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/date_time.hpp>
#include <iostream>
#include <stdlib.h>

using namespace util_ipc;
using namespace std;

#define SIZE   1024
#define N      10485760

void tolower( string &s) {
  size_t i;
  for(i=0; i<s.size(); ++i) {
    s[i] = (char)tolower(s[i]);
  }
}

int main( int argc, char *argv[]) 
{

  if (argc != 2) {
    cerr << "usage  test1_circular_queue_in_shared_memory  [parent|child|cleanup]" << endl;
    return 1;
  }

  string argv1( argv[1]);

  tolower( argv1);

  using namespace boost::interprocess;

  if (argv1 == "parent") {  //Parent process
    util_ipc::circular_queue_in_shared_memory<int64_t,SIZE> *cq;

    if (argv1 == "parent") {
      cq = util_ipc::circular_queue_in_shared_memory<int64_t,SIZE>::open_or_create( "MySharedMemory");
    } else {
      cq = util_ipc::circular_queue_in_shared_memory<int64_t,SIZE>::open_only( "MySharedMemory");
    }

    int i;
    for (i=0; i<N;) {
      if (cq->end() != 0) {
	*cq->end() = i;
	cq->push_back();
	++i;
      }
    }

    cout << "test passed in parent process" << endl;

    delete cq;

    return 0;
  } else  if (argv1 == "child") {
    util_ipc::circular_queue_in_shared_memory<int64_t,SIZE> *cq =
      util_ipc::circular_queue_in_shared_memory<int64_t,SIZE>::open_only( "MySharedMemory");
    
    int i;

    boost::posix_time::ptime t0 = boost::posix_time::microsec_clock::universal_time();
    boost::posix_time::ptime t1;

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

    t1 = boost::posix_time::microsec_clock::universal_time();

    boost::posix_time::time_duration dt = t1 - t0;

    cerr << "test passed in Child process!" << endl;

    double tm = dt.total_microseconds();
    tm /= (N * sizeof(int64_t));

    cout << "micro seconds / byte = " << tm << endl;

    return 0;
  } else if (argv1=="cleanup") {
    boost::interprocess::shared_memory_object::remove( "MySharedMemory");
  } else {
    cerr << "argv[1] = {" << argv1 << "} is not recognized, it should be parent or child" << endl;
    return 2;
  }
}
