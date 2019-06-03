#include <chrono>
#include <thread>
#include "architecture.h"
#include "shared_memory_price_server.h"

void init_snapshot(security_encoding* sec_codes, size_t num_sec_codes) {
}

int main() {
    volatile int stop_now = 0;
    security_encoding* sec_codes;
    size_t num_sec_codes;
    set_cpu_affinity(SHARED_MEMORY_PRICE_SERVER_CPU_AFFINITY);
    set_schedular_policy_fifo();
    set_signal_handlers(&stop_now);
    init_handshake_info(sec_codes, num_sec_codes);
    init_snapshot(sec_codes, num_sec_codes);
    //init simple snapshot
    while (true) {
        //increment heartbeats
        //update new price queue
        //update prices snapshot
#if SAVE_CPU
        this_thread::sleep_for(std::chrono::milliseconds(x));
#endif
        if (stop_now)
            break;
#ifdef ARCH_X86
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
}
        

