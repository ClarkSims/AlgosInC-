#include <chrono>
#include <thread>
#include <iostream>
#include <vector>
#include "architecture.h"
#include "shared_memory_price_server.h"

#define EXIT_EARLY false
#define SAVE_CPU_SLEEP_TIME 100

int main() {
    int stop_now = 0;
    volatile uint64_t *heartbeats;
    security_encoding* sec_codes;
    size_t num_sec_codes, stock_offset, num_stocks;
    set_cpu_affinity(SHARED_MEMORY_PRICE_SERVER_CPU_AFFINITY);
    set_schedular_policy_fifo();
    set_signal_handlers(&stop_now);
    init_handshake_info(sec_codes, num_sec_codes, true);
    guarded_price_datum *gpd = init_snapshot(sec_codes, num_sec_codes, true), 
        *pnt_nbbo;
    price_datum pd;
    num_stocks = num_sec_codes / NUM_STOCK_EXCHANGE;
    heartbeats = init_heartbeats();
    *heartbeats = 0;

    while (true) {
        ++*heartbeats;
        architecture_release_fence();

        stock_offset = rand() % num_stocks;
        pnt_nbbo = gpd + stock_offset * NUM_STOCK_EXCHANGE;
        if (!pnt_nbbo->try_copy_to_rhs_reverse_order( &pd, 1)) {
            std::cerr << "snapshot is corrupted!" << std::endl;
            break;
        }
        uint32_t mid = (pd.bid + pd.ask + 1) / 2;
        calc_new_market_with_random_price_change( 
            pnt_nbbo, 
            NUM_STOCK_EXCHANGE, 
            *heartbeats,
            mid);
            

        //std::vector<price_datum*>& changed_list = 
        //random_sleep_random_update_security((security_datum*)sec_data, num_sec_codes);

        // write this later
        //update new price queue

        // this conditional must be before pause
        if (stop_now)
            break;

#if SAVE_CPU_SLEEP_TIME > 0
        std::this_thread::sleep_for(std::chrono::milliseconds(SAVE_CPU_SLEEP_TIME));
#else
        architecture_pause();
#endif

#if EXIT_EARLY
        if  (*heartbeats > 10) break;
#endif
    }
    std::cerr << "shutting down" << std::endl;
}
        

