#include <iostream>
#ifdef __linux__
#include <sched.h>
#include <errno.h>
#include <stdio.h>
#include <pthread.h>
#endif
#include "shared_memory_price_server.h"

int set_schedular_policy_fifo() {
#ifdef __linux__

    struct sched_param sp;
    int policy;

    if((policy = sched_getscheduler(0) == -1)) {
      perror( "could not get policy");
      goto error;
    }

    if(policy == SCHED_OTHER) {
      sp.sched_priority = sched_get_priority_max(SCHED_FIFO);
      if (errno) {
        perror( "sched_get_priority_max failed");
        goto error;
      }
      sched_setscheduler(0, SCHED_FIFO, &sp);
      if (errno) {
        perror( "sched_setscheduler failed");
        goto error;
      }
    }
#endif
    return 0;

  error:
    using namespace std;
    cerr << "COULD NOT SET TASK SCHEDULER TO FIFO!! Perhaps permissions need to be changed?" << endl;
    return errno;
}
