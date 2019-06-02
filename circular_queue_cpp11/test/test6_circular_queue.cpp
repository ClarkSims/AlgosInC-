/// \file test6_circular_queue.cpp
///  \brief This program measures the transfer rate between two processes communicating data with a circular queue
///  in shared memory.
#include "circular_queue.h"

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
    cerr << "usage  test6_circular_queue  [parent|child]" << endl;
    return 1;
  }

  string argv1( argv[1]);

  tolower( argv1);

  using namespace boost::interprocess;

  if(argv1 == "parent"){  //Parent process
    //Remove shared memory on construction and destruction
    struct shm_remove 
    {
      shm_remove() { shared_memory_object::remove("test5_circular_queue"); }
      ~shm_remove(){ shared_memory_object::remove("test5_circular_queue"); }
    } remover;


    int32_t sz = sizeof( circular_queue<int,SIZE>);
    //Create a shared memory object.
    shared_memory_object shm ( open_or_create, "test5_circular_queue", read_write);

    //Set size
    shm.truncate(sz);

    //Map the whole shared memory in this process
    mapped_region region(shm, read_write);

    circular_queue<int,SIZE> *cq = 
      circular_queue<int,SIZE>::factory( region.get_address(), "test5_circular_queue");

    int i;
    for (i=0; i<N;) {
      if (cq->end() != 0) {
	*cq->end() = i;
	cq->push_back();
	++i;
      }
    }

    cout << "test passed in parent process" << endl;

    return 0;
  } else  if (argv1 == "child") {

    //Open already created shared memory object.
    shared_memory_object shm (open_only, "test5_circular_queue", read_write);

    //Map the whole shared memory in this process
    mapped_region region(shm, read_write);

    circular_queue<int,SIZE> *cq = static_cast<circular_queue<int,SIZE> *>(region.get_address());
    
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
    tm /= N * sizeof(int);

    cout << "micro seconds / byte = " << tm << endl;

    return 0;
  } else {
    cerr << "argv[1] = {" << argv1 << "} is not recognized, it should be parent or child" << endl;
    return 2;
  }
}
