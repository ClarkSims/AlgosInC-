/// \file test1_circular_queue_in_shared_memory.cpp
///  \brief This program measures the transfer rate between two processes communicating data with a circular queue
///  in shared memory. It uses the class circular_queue_in_shared_memory.
#include "circular_queue.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <iostream>
#include <stdlib.h>
#include <chrono>

using namespace util_ipc;
using namespace std;

// bigger than 1 small page, smaller than 1 huge page
#define SIZE   1024*32
#define SHM_SIZE SIZE*sizeof()
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
    void *data;
    int mode;
    typedef circular_queue<unsigned,SIZE> cqueue;
    size_t i, cqueue_size = sizeof(cqueue);
    cqueue *cq;

    if (argc != 2) {
        cerr << "usage test1_circular_queue_in_shared_memory  [parent|child|cleanup]" << endl;
        return 1;
    }

    string argv1( argv[1]);

    tolower( argv1);

    /* make the key: */
    if ((key = ftok("test1_circular_queue_in_shared_memory.cpp", 3141)) == -1) {
        perror("ftok");
        exit(1);
    }

    /* connect to (and possibly create) the segment: */
    if ((shmid = shmget(key, cqueue_size, 0644 | IPC_CREAT | SHM_HUGETLB)) == -1) {
        // for machines not configured for huge pages
        if ((shmid = shmget(key, cqueue_size, 0644 | IPC_CREAT)) == -1) {
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

    cq = cqueue::factory(data);

    if (argv1 == "parent") {  //Parent process
        
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
        std::chrono::high_resolution_clock::time_point t1, t0 = std::chrono::high_resolution_clock::now();

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

        t1 = std::chrono::high_resolution_clock::now();

        auto dt = t1 - t0;

        cerr << "test passed in Child process!" << endl;

        long long tm = std::chrono::duration_cast<std::chrono::microseconds>(dt).count();
        tm /= (N * sizeof(int64_t));

        cout << "micro seconds / byte = " << tm << endl;

        return 0;
    } else if (argv1=="cleanup") {
        //boost::interprocess::shared_memory_object::remove( "MySharedMemory");
    } else {
        cerr << "argv[1] = {" << argv1 << "} is not recognized, it should be parent or child" << endl;
        return 2;
    }
}
