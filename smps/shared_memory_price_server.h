#ifndef SHARED_MEMORY_PRICE_SERVER_H
#define SHARED_MEMORY_PRICE_SERVER_H

#define NUM_STOCK_EXCHANGE 5 
#define SHARED_MEMORY_PRICE_SERVER_CPU_AFFINITY 3
#define HANDSHAKE_ID 1234
#define HANDSHAKE_SIZE 65536
#define CIRCULAR_QUEUE_ID 2345

#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <vector>
#include "architecture.h"
#include "memory_fence.h"
#include "price_datum.h"
#include "security_encoding.h"

void set_cpu_affinity(int num);

int set_schedular_policy_fifo();

void termination_handler (int signum);

void set_signal_handlers(volatile int * flag);

extern unsigned num_stock_symbols;
extern size_t security_encoding_shmem_size;
extern const char *exchanges[NUM_STOCK_EXCHANGE];
extern const char *tickers[];
#define SECURITY_ENCODING_SHMEM_ID 1111
#define SECURITY_ENCODING_FILE_NAME "shared_memory_price_server_watermark.txt"

// populates shared memory file with list of exchanges, tickers, security ids, and tick denominators
// ie security encodings
void init_handshake_info(security_encoding*& sec_codes, size_t& num_sec_codes);

void *get_shared_memory_object( const char *fname, int id, size_t size, bool huge_page=false);

#define SECURITY_ENCODING_HEARTBEATS_ID 2222
volatile uint64_t *init_heartbeats();

/*
std::vector<price_datum*>& random_sleep_random_update_security(
    security_datum* sec_codes, size_t num_sec_codes);
*/

guarded_price_datum* init_snapshot(
    security_encoding* sec_codes,
    size_t num_sec_codes,
    bool first_time = 0);

#define SECURITY_SNAPSHOT_SHMEM_ID 3333

void calc_new_market_with_random_price_change(
    guarded_price_datum* pnt_nbbo,
    int      num_venue,
    uint64_t time,
    uint32_t midpoint);

#endif
