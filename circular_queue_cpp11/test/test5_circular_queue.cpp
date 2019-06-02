#ifdef _MSC_VER
#include <iostream>

int main() {
  std::cout << "this is a linux only program, because it uses fork" << std::endl;
  return 0;
}

#else

#include "circular_queue.h"
#include "CustomRand.h"

#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <iostream>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace util_ipc;
using namespace std;

#define SIZE   256
#define N      1024



int main( int argc, char *argv[]) 
{
  using namespace boost::interprocess;

  int pid = fork();

  if(pid != 0){  //Parent process
    //Remove shared memory on construction and destruction
    struct shm_remove 
    {
      shm_remove() { shared_memory_object::remove("MySharedMemory"); }
      ~shm_remove(){ shared_memory_object::remove("MySharedMemory"); }
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

    ax_b_c_random Rand;
    int i;
    unsigned int I;

    int stop_length = SIZE / 4;

    for (i=0; i<N;) {
      if (cq->end() != 0) {
	Rand.put_seed( i);
	I = Rand.rand();
	*cq->end() = I;
	cq->push_back();
	++i;
	if (i % stop_length == 0) {
	  sleep( 1);
	}
      }
    }

    cout << "test passed in parent process" << endl;

    int status;
    wait( &status);
    cerr << "child exited with status " << status << endl;

    return 0;
  } else  {

    // give parent time to start
    sleep( 1);

    //Open already created shared memory object.
    shared_memory_object shm (open_only, "test5_circular_queue", read_write);

    //Map the whole shared memory in this process
    mapped_region region(shm, read_write);

    circular_queue<int,SIZE> *cq = static_cast<circular_queue<int,SIZE> *>(region.get_address());

    ax_b_c_random Rand;
    
    int i, I;

    int stop_length = SIZE / 3;

    for (i=0; i<N; ) {
      if (cq->begin() != 0) {
	Rand.put_seed( i);
	I = Rand.rand();
	if (I != *cq->begin()) {
	  cerr << "test failed at offset i = " << i << " value = " << *cq->begin() << endl;
	  return 1;
	}
	cq->pop_front();
	++i;
	if (i % stop_length == 0) {
	  sleep( 1);
	}
      }
    }

    cout << "test passed in Child process!" << endl;
    return 0;
  }
}

#endif
