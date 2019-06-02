/// \file test1_circular_queue_in_shared_memory.cpp
///  \brief This program measures the transfer rate between two processes communicating data with a circular queue
///  in shared memory. It uses the class circular_queue_in_shared_memory.
#include "circular_queue.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <iostream>
#include <stdlib.h>

using namespace util_ipc;
using namespace std;

// bigger than 1 small page, smaller than 1 huge page
#define SIZE   1024*32
#define N      10485760

void tolower( string &s) {
  size_t i;
  for(i=0; i<s.size(); ++i) {
    s[i] = (char)tolower(s[i]);
  }
}

int main( int argc, char *argv[]) 
{
    key_t key;
    int shmid;
    char *data;
    int mode;

    if (argc != 2) {
        cerr << "usage test1_circular_queue_in_shared_memory  [parent|child|cleanup]" << endl;
        return 1;
    }

    string argv1( argv[1]);

    tolower( argv1);

    if (argv1 == "parent") {  //Parent process
        util_ipc::circular_queue<int64_t,SIZE> *cq;

        /* make the key: */
        if ((key = ftok("test1_circular_queue_in_shared_memory.cpp", 3141)) == -1) {
            perror("ftok");
            exit(1);
        }

        /* connect to (and possibly create) the segment: */
        if ((shmid = shmget(key, SHM_SIZE, 0644 | IPC_CREAT | SHM_HUGETLB)) == -1) {
            // for machines not configured for huge pages
            if ((shmid = shmget(key, SHM_SIZE, 0644 | IPC_CREAT)) == -1) {
                /* 0644 =>    drw-r--r-- */
                perror("shmget");
                exit(1);
            }
        }

        /* attach to the segment to get a pointer to it: */
        data = shmat(shmid, (void *)0, 0);
        if (data == (char *)(-1)) {
            perror("shmat");
            exit(1);
        } else {
            printf( "data pointer = %p\n", data);
        }

        circular_queue<unsigned int,SIZE> *cq = circular_queue<unsigned int,SIZE>::factory(data);

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
