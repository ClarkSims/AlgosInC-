#ifdef linux
#include <sched.h>
#include <errno.h>
#include <stdio.h>
#include <pthread.h>
#endif
#include "shared_memory_price_server.h"

void set_cpu_affinity( int cpu_offset) { 
#ifdef linux 
    int s; 
    cpu_set_t cpuset; 
    pthread_t thread; 
    thread = pthread_self();

    CPU_ZERO(&cpuset);
    CPU_SET( cpu_offset, &cpuset);

    s = pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset);

    if (s != 0) {
        perror( "pthread_setaffinity_np failed ");
        cerr << "s = " << s << endl;
        exit(1);
    }
#endif
}

