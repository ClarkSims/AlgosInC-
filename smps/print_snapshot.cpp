#include <chrono>
#include <thread>
#include <iostream>
#include "architecture.h"
#include "shared_memory_price_server.h"

#define EXIT_EARLY true
#define SAVE_CPU_SLEEP_TIME 100

int main() {
    volatile uint64_t *heartbeats;
    security_encoding* sec_codes;
    size_t num_sec_codes;

    init_handshake_info(sec_codes, num_sec_codes);
    guarded_price_datum *gpd = init_snapshot(sec_codes, num_sec_codes);
    heartbeats = init_heartbeats();

    using namespace std;
    uint64_t begin_heartbeats = *heartbeats;
    architecture_aquire_fence();
    print_snapshot(gpd, sec_codes, num_sec_codes);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    architecture_aquire_fence();
    uint64_t end_heartbeats = *heartbeats;
    if (begin_heartbeats == end_heartbeats) {
        std::cerr << "begin_heartbeats == end_heartbeats" << std::endl;
        std::cerr << "price server might be dead!" << std::endl;
        return 1;
    } else {
        std::cout << "number of heartbeats during print = "
            << end_heartbeats - begin_heartbeats << endl;
    }
}
        

