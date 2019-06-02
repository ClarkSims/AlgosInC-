/// \file test3_circular_queue_in_shared_memory.cpp
/// checks for memory leaks. This program should be run with valgrind as such:
/// valgrind --leak-check=full ../bin/test3_circular_queue_in_shared_memory
#include "circular_queue_in_shared_memory.h"

#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/date_time.hpp>
#include <iostream>
#include <stdlib.h>

using namespace util_ipc;
using namespace std;

#define SIZE   1024

int main( int argc, char *argv[]) 
{
  util_ipc::circular_queue_in_shared_memory<int64_t,SIZE> *cq;

  cq = util_ipc::circular_queue_in_shared_memory<int64_t,SIZE>::circular_queue_on_heap( "test3_cqsm");

  cq->remove();
  delete cq;

  return 0;
}
