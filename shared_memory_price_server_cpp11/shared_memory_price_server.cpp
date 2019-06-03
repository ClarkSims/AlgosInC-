#define CPU_NUM 3

void set_cpu_affinity(int num) {
}

void set_schedular_policy() {
}

void set_signal_handlers(volatile int * flag) {
}

struct security_encoding {
    char ticker[32];
    char exchange[8];
    uint32_t id;
    uint32_t denominator;
};

// populates shared memory file with list of exchanges, tickers, security ids, and tick denominators
// ie security encodings
void init_handshake_info() {
}

#ifdef ARCH_X86
inline void pause() {
#if defined(_MSC_VER)
    atomic_signal_fence(memory_order_acq_rel);
    __asm { pause }
    atomic_signal_fence(memory_order_acq_rel);
#elif defined(__GNUC__) || defined( __ICL)
    __asm__ __volatile__ ("pause" ::: "memory");
#else
    #error Unsupported Compiler
#endif
#else
inline void pause() {
}
#endif


int main() {
    volatile int stop_now = 0;
    set_cpu_affinity(CPU_NUM);
    set_schedular_policy();
    set_signal_handler(&stop_now);
    init_handshake_info();

    //init simple init handshake info
    //init simple snapshot
    while (true) {
        //increment heartbeats
        //update new price queue
        //update prices snapshot
#if SAVE_CPU
        sleep(1);
#else
        if(stop_now)
            break;
        // play nice with cpu by turning of branching if possible
        // https://stackoverflow.com/questions/50428450/what-does-asm-volatile-pause-memory-do
        // https://stackoverflow.com/questions/4725676/how-does-x86-pause-instruction-work-in-spinlock-and-can-it-be-used-in-other-sc
#ifdef ARCH_X86
#if defined(_MSC_VER)
        atomic_signal_fence(memory_order_acq_rel);
        __asm { pause }
        atomic_signal_fence(memory_order_acq_rel);
#elif defined(__GNUC__) || defined( __ICL)
        __asm__ __volatile__ ("pause" ::: "memory");
#else
#error Unsupported Compiler, write pause if possible
#endif
#else
#error Unsuppoted OS, write pause if possible
#endif

    }
}
        
