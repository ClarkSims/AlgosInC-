#include <chrono>
#include <thread>
#include <iostream>
#include "architecture.h"
#include "shared_memory_price_server.h"
#include "memory_fence.h"

#define SAVE_CPU_SLEEP_TIME 100

void init_snapshot(security_encoding* sec_codes, size_t num_sec_codes) {

}

size_t random_sleep_random_update_security(security_encoding* sec_codes, size_t num_sec_codes) {
    return 1;
}

int main() {
    volatile int stop_now = 0;
    volatile uint64_t *heartbeats;
    security_encoding* sec_codes;
    size_t num_sec_codes;
    set_cpu_affinity(SHARED_MEMORY_PRICE_SERVER_CPU_AFFINITY);
    set_schedular_policy_fifo();
    set_signal_handlers(&stop_now);
    init_handshake_info(sec_codes, num_sec_codes);
    init_snapshot(sec_codes, num_sec_codes);
    heartbeats = init_heartbeats();
    //init simple snapshot
    while (true) {
        ++*heartbeats;
#if defined(ARCH_X86)
        memory_fence::sfence();
#else
        atomic_thread_fence(std::memory_order_release);
#endif
        random_sleep_random_update_security(sec_codes, num_sec_codes);

        //update new price queue
        //update nbbo

        // this conditional must be before pause
        if (stop_now)
            break;

#if SAVE_CPU_SLEEP_TIME > 0
        std::this_thread::sleep_for(std::chrono::milliseconds(SAVE_CPU_SLEEP_TIME));
#elif defined(ARCH_X86)
#if defined(_MSC_VER)
        atomic_signal_fence(memory_order_acq_rel);
        __asm { pause }
        atomic_signal_fence(memory_order_acq_rel);
#elif defined(__GNUC__) || defined( __ICL)
        __asm__ __volatile__ ("pause" ::: "memory");
#else
#error Unsupported Compiler, write pause if possible
#endif // defined(_MSC_VER)
#else
#error Unsuppoted OS, write pause if possible
#endif // ARCH_X86
    }
    std::cerr << "shutting down" << std::endl;
}
        

