#ifndef SHARED_MEMORY_PRICE_SERVER_H
#define SHARED_MEMORY_PRICE_SERVER_H

#include <stdint.h>

void set_cpu_affinity(int num);

void set_schedular_policy();

void termination_handler (int signum);

void set_signal_handlers(volatile int * flag);

struct security_encoding {
    char ticker[32];
    char exchange[8];
    uint32_t id;
    uint32_t denominator;
};

// populates shared memory file with list of exchanges, tickers, security ids, and tick denominators
// ie security encodings
void init_handshake_info();
#endif
