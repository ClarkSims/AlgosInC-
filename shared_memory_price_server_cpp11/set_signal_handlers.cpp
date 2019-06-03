#include <signal.h>
#include "shared_memory_price_server.h"

volatile int * global_stop_now = nullptr;

void termination_handler (int signum) {
    *global_stop_now = signum;
}

void set_signal_handlers(volatile int * flag) {
    struct sigaction new_action, old_action;
    new_action.sa_handler = termination_handler;
    sigemptyset (&new_action.sa_mask);
    new_action.sa_flags = 0;
    sigaction (SIGINT, nullptr, &old_action);
    if (old_action.sa_handler != SIG_IGN)
        sigaction (SIGINT, &new_action, nullptr);
    sigaction (SIGHUP, nullptr, &old_action);
    if (old_action.sa_handler != SIG_IGN)
        sigaction (SIGHUP, &new_action, nullptr);
    sigaction (SIGTERM, nullptr, &old_action);
    if (old_action.sa_handler != SIG_IGN)
        sigaction (SIGTERM, &new_action, nullptr);

}

