#include <stdlib.h>
#include <thread>
#include <vector>
#include "shared_memory_price_server.h"

std::vector<price_datum*> dirty_list(NUM_STOCK_EXCHANGE);

std::vector< price_datum*>& mark_all_dirty( size_t sec_off,  security_datum* sec_data) {
    security_datum* p_stock = sec_data + sec_off*NUM_STOCK_EXCHANGE;
    for (unsigned ex_off = 0; ex_off < NUM_STOCK_EXCHANGE; ++ex_off) {
        dirty_list.at(ex_off) = (price_datum*) p_stock[ex_off].get_older();
        dirty_list.at(ex_off)->mark_dirty();
    }
    return dirty_list;
}

void define_market( std::vector<price_datum*>& dirty_list, unsigned mid)
{
    uint32_t nbbo_bid=0, nbbo_ask=UINT32_MAX;
    dirty_list.at(0)->bid = mid - 1 - 10;
    dirty_list.at(0)->ask = mid + 1 - 10;
    dirty_list.at(0)->bid_size = 10;
    dirty_list.at(0)->ask_size = 10;

    for (unsigned ex_off = 1; ex_off < NUM_STOCK_EXCHANGE; ++ex_off) {
        dirty_list.at(ex_off)->bid = mid - ex_off - 10;
        dirty_list.at(ex_off)->ask = mid + ex_off - 10;
        dirty_list.at(ex_off)->bid_size = 10;
        dirty_list.at(ex_off)->ask_size = 10;
    }

}

void mark_all_clean( std::vector<price_datum*>& dirty_list) {
    for (unsigned ex_off = 1; ex_off < NUM_STOCK_EXCHANGE; ++ex_off) {
        dirty_list[ex_off]->mark_clean();
    }
    dirty_list[0]->mark_clean();
}

std::vector<price_datum*>& random_sleep_random_update_security(
    security_datum* sec_data,
    size_t num_sec_datum)
{
    for (auto ex_off=0; ex_off < NUM_STOCK_EXCHANGE; ++ex_off) {
        dirty_list[ex_off] = nullptr;
    }

    size_t sleeptime = random() % 10;
    if (sleeptime>0)
        std::this_thread::sleep_for(std::chrono::milliseconds(sleeptime));
    bool do_trade = random() % 5 == 4;
    if (do_trade) {
        size_t sec_off = random() % num_sec_datum;
        size_t ex_off = random() % (NUM_STOCK_EXCHANGE - 1) + 1; // skip nbbo
         security_datum* nbbo_update = &sec_data[sec_off*NUM_STOCK_EXCHANGE];

        // ugly! fix this, did this because I was getting compile errors
        // used excessive force to insure against reordering
        price_datum* nbbo_pd = (price_datum*) ((security_datum*) nbbo_update)->get_older();

         security_datum* update = &sec_data[sec_off*NUM_STOCK_EXCHANGE + ex_off];
        // ugly! fix this, did this because I was getting compile errors
        // used excessive force to insure against reordering
        price_datum* pd = (price_datum*) ((security_datum*) nbbo_update)->get_older();

        size_t tick_size = random() % 7 - 3;
        size_t bo_spread = random() %5 + 5;
        int new_bid = pd->bid + tick_size;
        if (new_bid > 0 ) {
            int new_ask = pd->bid + bo_spread;
            int new_bid_size = random() % 7 + 1;
            int new_ask_size = random() % 7 + 1;

            if ((unsigned)new_bid > nbbo_pd->ask || (unsigned)new_ask < nbbo_pd->bid) {
                if ((unsigned)new_bid < (unsigned)new_ask) {
                    // mark all data
                    mark_all_dirty( sec_off, sec_data);
                    // define market from midpoint
                    //define_market( sec_off, sec_data, (new_bid +new_ask)/2);
                    // cleanup
                    //mark_all_clean( sec_off, sec_data);
                }

            } else {
                bool nbbo_dirty = false;
                if (pd->bid > nbbo_pd->bid || pd->ask < nbbo_pd->ask){
                    nbbo_pd->mark_dirty();
                    atomic_signal_fence(std::memory_order_acq_rel);
                    nbbo_dirty = true;
                }
                pd->mark_dirty();
                atomic_signal_fence(std::memory_order_acq_rel);

                pd->bid = new_bid;
                pd->ask = new_ask;
                pd->bid_size = new_bid_size;
                pd->ask_size = new_ask_size;

                pd->mark_clean();
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
    return dirty_list;
}
