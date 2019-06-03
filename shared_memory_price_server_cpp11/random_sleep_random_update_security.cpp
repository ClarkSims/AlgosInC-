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

        // ugly! fix this, did this because I was getting compile errors
        // used excessive force to insure against reordering
        price_datum* nbbo_pd = (price_datum*) ((security_datum*) nbbo_update)->get_older();

        volatile security_datum* update = &sec_datum[sec_off*NUM_STOCK_EXCHANGE + ex_off];
        // ugly! fix this, did this because I was getting compile errors
        // used excessive force to insure against reordering
        price_datum* pd = (price_datum*) ((security_datum*) nbbo_update)->get_older();
        pd->mark_dirty();
        atomic_signal_fence(std::memory_order_acq_rel);

        size_t tick_size = random() % 7 - 3;
        size_t bo_spread = random() %5 + 5;
        int new_bid = pd->bid + tick_size;
        if (new_bid > 0 ) {
            int new_ask = pd->bid + bo_spread;
            int new_bid_size = random() % 7 + 1;
            int new_ask_size = random() % 7 + 1;

            if (new_bid > nbbo_pd->ask || new_ask < nbbo_pd->bid) {
                // invalidate everyting
                // set to midpoint
            } else {
                bool nbbo_dirty = false;
                if (pd->bid > nbbo_pd->bid || pd->ask < nbbo_pd->ask){
                    nbbo_pd->mark_dirty();
                    atomic_signal_fence(std::memory_order_acq_rel);
                    nbbo_dirty = true;
                }
                pd->bid = new_bid;
                pd->ask = new_ask;
                pd->bid_size = new_bid_size;
                pd->ask_size = new_ask_size;

                // again ugly, fix later
                atomic_signal_fence(std::memory_order_acq_rel);
                pd->mark_clean();
                atomic_signal_fence(std::memory_order_acq_rel);
                if (pd->bid > nbbo_pd->bid) {
                    nbbo_pd->bid = new_bid;
                    nbbo_pd->bid_size = new_bid_size;
                }
                if (pd->ask < nbbo_pd->ask) {
                    nbbo_pd->ask = new_ask;
                    nbbo_pd->ask_size = new_ask_size;
                }

                // again ugly, fix later
                atomic_signal_fence(std::memory_order_acq_rel);
                if (nbbo_dirty) {
                    nbbo_pd->mark_clean();
                }
            }
        }
    }
    return 1;
}
