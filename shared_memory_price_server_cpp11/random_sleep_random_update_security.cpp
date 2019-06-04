#include <stdlib.h>
#include <thread>
#include <vector>
#include "shared_memory_price_server.h"

std::vector<price_datum*> dirty_list(NUM_STOCK_EXCHANGE, nullptr);

std::vector< price_datum*>& mark_all_dirty( size_t stock_number,  security_datum* sec_data) {
    security_datum* p_stock = sec_data + stock_number*NUM_STOCK_EXCHANGE;
    for (unsigned ex_off = 0; ex_off < NUM_STOCK_EXCHANGE; ++ex_off) {
        dirty_list.at(ex_off) = (price_datum*) p_stock[ex_off].get_older();
        dirty_list.at(ex_off)->mark_dirty();
    }
    return dirty_list;
}

void define_market( std::vector<price_datum*>& dirty_list, unsigned mid)
{
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

    // set back to null
    for (auto ex_off=0; ex_off < NUM_STOCK_EXCHANGE; ++ex_off) {
      if (dirty_list[ex_off] == nullptr) {
	break;
      } else {
        dirty_list[ex_off] = nullptr;
      }
    }
}

volatile security_datum* init_snapshot(
    security_encoding* sec_codes,
    size_t num_sec_codes,
    bool first_time) 
{
    void *data = get_shared_memory_object(SECURITY_ENCODING_FILE_NAME, SECURITY_SNAPSHOT_SHMEM_ID, 64);
    volatile security_datum* sec_data;
    sec_data = reinterpret_cast<volatile security_datum *>(data);

    if (first_time) {
        size_t num_tickers = num_sec_codes / NUM_STOCK_EXCHANGE;
        for (size_t i=0; i<num_tickers; ++i) {
            mark_all_dirty( i, (security_datum *)sec_data);
            define_market( dirty_list, 1000000);
            mark_all_clean( dirty_list);            
        }
    }

    return sec_data;
}

std::vector<price_datum*>& random_sleep_random_update_security(
    security_datum* sec_data,
    size_t num_sec_datum)
{
    size_t sleeptime = random() % 10;
    if (sleeptime>0)
        std::this_thread::sleep_for(std::chrono::milliseconds(sleeptime));
    bool do_trade = random() % 5 == 4;
    if (do_trade) {
        size_t num_stocks = num_sec_datum / NUM_STOCK_EXCHANGE;
        size_t stock_offset = random() % num_stocks;
        size_t ex_off = random() % (NUM_STOCK_EXCHANGE - 1) + 1; // skip nbbo
        security_datum* nbbo_update = &sec_data[stock_offset*NUM_STOCK_EXCHANGE];

        price_datum* nbbo_pd = (price_datum*) ((security_datum*) nbbo_update)->get_older();

        security_datum* update = &sec_data[stock_offset*NUM_STOCK_EXCHANGE + ex_off];

        price_datum* pd = (price_datum*) ((security_datum*) update)->get_older();

        size_t tick_size = random() % 7 - 3;
        size_t bo_spread = random() % 5 + 5;
        int new_bid = pd->bid + tick_size;
        if (new_bid > 0 ) {
            int new_ask = pd->bid + bo_spread;
            int new_bid_size = random() % 7 + 1;
            int new_ask_size = random() % 7 + 1;

            if ((unsigned)new_bid > nbbo_pd->ask || (unsigned)new_ask < nbbo_pd->bid) {
                if ((unsigned)new_bid < (unsigned)new_ask) {
                    // mark all data
                    mark_all_dirty( stock_offset, sec_data);
                    // define market from midpoint
                    define_market( dirty_list, (new_bid +new_ask)/2);
                    // cleanup
                    mark_all_clean( dirty_list);
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

                if (nbbo_dirty) {
                    nbbo_pd->mark_clean();
                }
            }
        }
    }
    return dirty_list;
}
