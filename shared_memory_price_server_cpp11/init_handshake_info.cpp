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
    size_t i, j, k;

    num_stock_symbols = 0;
    for (i=0; tickers[i][0]!=0; ++i) {}
    num_stock_symbols = i;
    num_sec_codes = num_stock_symbols * NUM_STOCK_EXCHANGE;

    size_t min_shmem_size = num_stock_symbols * NUM_STOCK_EXCHANGE * sizeof(security_encoding);
    assert(min_shmem_size < security_encoding_shmem_size);

    void *data = get_shared_memory_object(SECURITY_ENCODING_FILE_NAME, SECURITY_ENCODING_SHMEM_ID, security_encoding_shmem_size);
    sec_codes = reinterpret_cast<security_encoding*>( data);

    for (i=k=0; tickers[i][0]!=0; ++i) {
        for (j=0; exchanges[j]!=0; ++j, ++k) {
            char* ptr = reinterpret_cast<char*>(&sec_codes[k]);
            security_encoding *dummy = new security_encoding(tickers[i], exchanges[j], k, 100);
            // all trading is in pennies for starters
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

