#ifndef atomic_ioguard_H
#define atomic_ioguard_H

#include <stdint.h>
#include <algorithm>
#include <atomic>

#include "architecture.h"

#include <atomic>

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
    
        atomic_ioguard( const U& data) 
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
            ++(guard_type&)guard.front;
            architecture_aquire_fence(); // mark *this as unstable
            const volatile uint64_t *End = (const volatile uint64_t *)rhs+num_64bit_reads;
            std::copy( (const volatile uint64_t *)rhs, End, (uint64_t*)&data);
            architecture_release_fence(); // mark *this as stable
            (guard_type&)guard.back = (guard_type&)guard.front;
        }
   
        void init( const U &rhs) {
            init( &rhs);
        }

        void copy_to_rhs( atomic_ioguard* rhs) const {
            ++rhs->front_guard;
            architecture_release_fence(); // mark rhs as unstable
            std::copy( (const volatile uint64_t*)&data, end(), (uint64_t*)&rhs->data);
            architecture_release_fence(); // mark rhs as stable
            rhs->back_guard = rhs->front_guard;
        }

        static void copy_to_rhs( const U& data, atomic_ioguard* rhs) {
            ++rhs->front_guard;
            architecture_release_fence(); // mark rhs as unstable
            std::copy( (const volatile uint64_t*)&data, 
                    (const volatile uint64_t*)&data + num_64bit_reads, 
                    (uint64_t*)&rhs->data);
            architecture_release_fence(); // mark rhs as stable
            rhs->back_guard = rhs->front_guard;
        }

        void copy_to_rhs( atomic_ioguard& rhs) const {
            copy_to_rhs( &rhs);
        }

        void init_reverse_order( const volatile atomic_ioguard* rhs) {
            do {
                back_guard = rhs->back_guard;
                std::copy( (const volatile uint64_t*)&rhs->data, rhs->end(), (uint64_t*)&data);
                architecture_aquire_fence();
                front_guard = rhs->front_guard;
            } while (front_guard != back_guard);
        }

        void init_reverse_order( const volatile atomic_ioguard& rhs) {
            init_reverse_order( &rhs);
        }

        void copy_to_rhs_reverse_order( U *rhs) const volatile {
            uint64_t Back_guard;
            do {
                Back_guard = back_guard;
                std::copy( (const volatile uint64_t *)&data, end(), (uint64_t*)rhs);
                architecture_aquire_fence();
            } while (front_guard != Back_guard);
        }

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

        void copy_to_rhs_reverse_order( U &rhs) const volatile {
            copy_to_rhs_reverse_order( &rhs);
        }
    };
}

#endif
