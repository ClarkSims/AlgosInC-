#ifndef SHARED_MEMORY_PRICE_SERVER_H
#define SHARED_MEMORY_PRICE_SERVER_H

#define SHARED_MEMORY_PRICE_SERVER_CPU_AFFINITY 3
#define HANDSHAKE_ID 1234
#define HANDSHAKE_SIZE 65536
#define CIRCULAR_QUEUE_ID 2345

#include <stdint.h>

void set_cpu_affinity(int num);

int set_schedular_policy_fifo();

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
void init_handshake_info(security_encoding*& sec_codes, size_t& num_sec_codes);

void *get_shared_memory_object( const char *fname, int id, size_t size);
#endif
