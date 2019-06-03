#ifdef __linux__
#include <sched.h>
#include <errno.h>
#include <stdio.h>
#include <pthread.h>
#endif
#include <iostream>
#include "shared_memory_price_server.h"

void set_cpu_affinity( int cpu_offset) { 
#ifdef __linux__
    int s; 
    cpu_set_t cpuset; 
    pthread_t thread; 
    thread = pthread_self();

    CPU_ZERO(&cpuset);
    CPU_SET( cpu_offset, &cpuset);

    s = pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset);

    if (s != 0) {
        perror( "pthread_setaffinity_np failed ");
        std::cerr << "s = " << s << std::endl;
        exit(1);
    }
#endif
}

