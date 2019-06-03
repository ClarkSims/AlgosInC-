#include <iostream>
#ifdef __linux__
#include <sched.h>
#include <errno.h>
#include <stdio.h>
#include <pthread.h>
#endif
#include "shared_memory_price_server.h"

void set_schedular_policy() {
#ifdef __linux__
    cpu_set_t mask;
    int ret;

    CPU_ZERO(&mask);
    CPU_SET((int)cpu, &mask);
    ret = sched_setaffinity(0, sizeof mask, &mask);

    if (ret!=0) {
      cerr << "COULD NOT SET CPU AFFINITY, ret = " << ret << endl;
    }
#endif
}
