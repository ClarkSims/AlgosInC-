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

void set_cpu_affinity(int num);

int set_schedular_policy_fifo();

void termination_handler (int signum);

void set_signal_handlers(volatile int * flag);

struct security_encoding {
    char ticker[8];
    char exchange[8];
    uint32_t id;
    uint32_t denominator;
    char padding[8];

    security_encoding() {
        memset( this, 0, sizeof(*this));
    }

    void init(
       const char *Ticker,
        const char *Exchange,
        uint32_t Id,
        uint32_t Denom)
    {
        assert( strlen(Ticker) < sizeof(ticker));
        assert( strlen(Exchange) < sizeof(exchange));
        strcpy( ticker, Ticker);
        strcpy( exchange, Exchange);
        id = Id;
        denominator = Denom;
    }

    security_encoding(
        const char *Ticker,
        const char *Exchange,
        uint32_t Id=0,
        uint32_t Denom=100):
        id(Id),
        denominator(Denom)
    {
        assert( strlen(Ticker) < sizeof(ticker));
        assert( strlen(Exchange) < sizeof(exchange));
        strcpy( ticker, Ticker);
        strcpy( exchange, Exchange);
    }
};

extern unsigned num_stock_symbols;
extern size_t security_encoding_shmem_size;
extern const char *exchanges[NUM_STOCK_EXCHANGE];
extern const char *tickers[];
#define SECURITY_ENCODING_FILE_NAME "security_encoding_watermark.txt"
#define SECURITY_ENCODING_SHMEM_ID 1111

// populates shared memory file with list of exchanges, tickers, security ids, and tick denominators
// ie security encodings
void init_handshake_info(security_encoding*& sec_codes, size_t& num_sec_codes);

void *get_shared_memory_object( const char *fname, int id, size_t size, bool huge_page=false);
#endif
