#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "shared_memory_price_server.h"

// populates shared memory file with list of exchanges, tickers, security ids, and tick denominators
// ie security encodings
void init_handshake_info(security_encoding*& sec_codes, size_t& num_sec_codes) {
    key_t key;
    int shmid;
    void *data;
    size_t i;

}

char *exchanges[] = {
    "ARCA",
    "BATS",
    "NASDAQ",
    "NYSE"
    ""
};

char *tickers[] = {
    "SPY",
    "QQQ",
    "MSFT",
    "INTC",
    "GE",
    "TEVA",
    ""
};

