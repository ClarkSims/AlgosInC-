#include <chrono>
#include <thread>
#include <iostream>
#include <vector>
#include "architecture.h"
#include "shared_memory_price_server.h"

#define SAVE_CPU_SLEEP_TIME 100

int main() {
    int stop_now = 0;
    volatile uint64_t *heartbeats;
    security_encoding* sec_codes;
    size_t num_sec_codes;
    set_cpu_affinity(SHARED_MEMORY_PRICE_SERVER_CPU_AFFINITY);
    set_schedular_policy_fifo();
    set_signal_handlers(&stop_now);
    init_handshake_info(sec_codes, num_sec_codes);
    guarded_price_datum *gpd = init_snapshot(sec_codes, num_sec_codes, true);
    heartbeats = init_heartbeats();

    while (true) {
        ++*heartbeats;
        architecture_release_fence();

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
    }
    std::cerr << "shutting down" << std::endl;
}
        

