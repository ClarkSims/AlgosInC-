#ifndef atomic_ioguard_H
#define atomic_ioguard_H

#include <stdint.h>
#include <algorithm>
#include <atomic>

#include "architecture.h"

namespace util_ipc {

  template
    <
      typename U, 
      typename guard_type=uint64_t,
      int num_64bit_reads = sizeof(U)/sizeof(uint64_t)
    >
    class atomic_ioguard {
        static_assert(sizeof(U) % sizeof(uint64_t) == 0,
          "fast copy requires size to be even multiple of 64 bits");
        static_assert(std::is_trivially_copyable<U>::value,
          "must be trivially copyable");
        inline volatile uint64_t* end() volatile { 
            return (uint64_t*)&data + num_64bit_reads;
        }

        inline const volatile uint64_t* end() const volatile { 
            return (uint64_t*)&data + num_64bit_reads;
        }

  public:
        struct guard_t {
          guard_type front = 0;
          guard_type back = 0;
        };
        std::atomic<guard_t> guard;
        U data;
    
        atomic_ioguard( const U& data) : data(data)
        {
        }

        atomic_ioguard()
        {
        }
    
        U& get_data() {
            return data;
        }

        const U& get_data() const {
            return data;
        }
    
        void init( const U* rhs) {
            guard_t local_guard = guard.load(std::memory_order_relaxed);

            // mark *this as dirty for readers
            ++local_guard.front;
            guard.store(local_guard,std::memory_order_relaxed);
            architecture_release_fence();

            // note this_thread is the only writer, so data is always clean
            const volatile uint64_t *End = (const volatile uint64_t *)rhs+num_64bit_reads;
            std::copy( (const volatile uint64_t *)rhs, End, (uint64_t*)&data);

            // mark *this as clean for readers
            architecture_release_fence();
            local_guard.back = local_guard.front;
            guard.store(local_guard,std::memory_order_relaxed);
        }
   
        void init( const U &rhs) {
            init( &rhs);
        }

        void copy_to_rhs( atomic_ioguard* rhs) const {
            rhs->init(&data);
        }

        static void copy_to_rhs( const U& data, atomic_ioguard* rhs) {
            rhs->init(&data);
        }

        void copy_to_rhs( atomic_ioguard& rhs) const {
            rhs->init(&data);
        }

        void init_reverse_order( const volatile atomic_ioguard* rhs) {
            guard_t local_guard;
            guard_type back_guard;
            while (true) {
                local_guard = guard.load(std::memory_order_relaxed);
                back_guard = local_guard.back;

                if (architecture_likely(local_guard.front == local_guard.back)) {
                    architecture_aquire_fence();
                    std::copy( (const volatile uint64_t*)&rhs->data, rhs->end(), (uint64_t*)&data);

                    local_guard = guard.load(std::memory_order_relaxed);
                    // todo research adding pause somewhere
                    if (architecture_likely(local_guard.front == back_guard)) {
                        break;
                    }
                }
            }
        }

        void init_reverse_order( const volatile atomic_ioguard& rhs) {
            init_reverse_order( &rhs);
        }

        void copy_to_rhs_reverse_order( U *rhs) const volatile {
            guard_t local_guard;
            guard_type back_guard;
            while (true) {
                local_guard = guard.load(std::memory_order_relaxed);
                back_guard = local_guard.back;

                if (architecture_likely(local_guard.front == local_guard.back)) {
                    architecture_aquire_fence();
                    std::copy( (const volatile uint64_t*)&data, end(), (uint64_t*)rhs);

                    local_guard = guard.load(std::memory_order_relaxed);
                    // todo research adding pause somewhere
                    if (architecture_likely(local_guard.front == back_guard)) {
                        break;
                    }
                }
            }
        }

#if 0
        bool try_copy_to_rhs_reverse_order( U *rhs, size_t num_try) const volatile {
            uint64_t Back_guard;
            for (size_t try_iter=0; try_iter<num_try; ++num_try) {
                Back_guard = back_guard;
                std::copy( (const volatile uint64_t *)&data, end(), (uint64_t*)rhs);
                architecture_aquire_fence();
                if (front_guard == Back_guard) {
                    return true;
                }
            }
            return false;
        }
#endif

        void copy_to_rhs_reverse_order( U &rhs) const volatile {
            copy_to_rhs_reverse_order( &rhs);
        }
    };
}

#endif
