#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>

#include "shared_memory_price_server.h"

// populates shared memory file with list of exchanges, tickers, security ids, and tick denominators
// ie security encodings
void init_handshake_info(security_encoding*& sec_codes, size_t& num_sec_codes) {
    size_t i;

}

const char *exchanges[] = {
    "ARCA",
    "BATS",
    "NASDAQ",
    "NYSE"
    ""
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

