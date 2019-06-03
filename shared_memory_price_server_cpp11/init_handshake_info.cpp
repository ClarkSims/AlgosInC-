#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <assert.h>

#include "shared_memory_price_server.h"

unsigned num_stock_symbols;
size_t security_encoding_shmem_size = 16384*sizeof(security_encoding)*NUM_STOCK_EXCHANGE;

// populates shared memory file with list of exchanges, tickers, security ids, and tick denominators
// ie security encodings
void init_handshake_info(security_encoding*& sec_codes, size_t& num_sec_codes) {
    size_t i, j;

    num_stock_symbols = 0;
    for (i=0; tickers[i][0]!=0; ++i) {}
    num_stock_symbols = i;

    size_t min_shmem_size = num_stock_symbols * NUM_STOCK_EXCHANGE * sizeof(security_encoding);
    assert(min_shmem_size < security_encoding_shmem_size);


    for (i=0; tickers[i][0]!=0; ++i) {
        for (j=0; exchanges[j]!=0; ++j) {

        }
    }
}

const char *exchanges[NUM_STOCK_EXCHANGE] = {
    "NBBO",
    "ARCA",
    "BATS",
    "NASDAQ",
    "NYSE"
};

const char *tickers[] = {
    "SPY",
    "QQQ",
    "MSFT",
    "INTC",
    "GE",
    "TEVA",
    ""
};

