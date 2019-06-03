#include <stdlib.h>
#include <thread>
#include "shared_memory_price_server.h"

size_t random_sleep_random_update_security(security_encoding* sec_codes, size_t num_sec_codes) {
    size_t sleeptime = random() % 10;
    if (sleeptime>0)
        std::this_thread::sleep_for(std::chrono::milliseconds(sleeptime));
    bool do_trade = random() % 5 == 4;
    if (do_trade) {
        size_t sec_off = random() % num_sec_codes;
        size_t ex_off = random() % NUM_STOCK_EXCHANGE;
        security_encoding* update = &sec_codes[sec_off*NUM_STOCK_EXCHANGE + ex_off];
        size_t tick_size = random() % 7 - 3;
        // arer h
    }
    return 1;
}
