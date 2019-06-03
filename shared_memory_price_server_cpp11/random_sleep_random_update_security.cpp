#include <stdlib.h>
#include <thread>
#include "shared_memory_price_server.h"

size_t random_sleep_random_update_security(volatile security_datum* sec_datum, size_t num_sec_datum) {
    size_t sleeptime = random() % 10;
    if (sleeptime>0)
        std::this_thread::sleep_for(std::chrono::milliseconds(sleeptime));
    bool do_trade = random() % 5 == 4;
    if (do_trade) {
        size_t sec_off = random() % num_sec_datum;
        size_t ex_off = random() % (NUM_STOCK_EXCHANGE - 1) + 1; // skip nbbo
        volatile security_datum* nbbo_update = &sec_datum[sec_off*NUM_STOCK_EXCHANGE];


        volatile security_datum* update = &sec_datum[sec_off*NUM_STOCK_EXCHANGE + ex_off];
        size_t tick_size = random() % 7 - 3;
        volatile price_datum* pd = 
            (update->first.front_guard >= update->second.front_guard)?
             &update->first : &update->second;

    }
    return 1;
}
