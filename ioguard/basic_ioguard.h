#ifndef BASIC_IOGUARD_H
#define BASIC_IOGUARD_H

#include <stdint.h>
#include <algorithm>

#include "architecture.h"

#include <atomic>

namespace util_ipc {

  template
    <
      typename U, 
      typename guard_type=uint64_t,
      int num_64bit_reads = sizeof(U)/sizeof(uint64_t)
    >
    class basic_ioguard {
        inline volatile uint64_t* end() volatile { 
            return (uint64_t*)&data + num_64bit_reads;
        }

        inline const volatile uint64_t* end() const volatile { 
            return (uint64_t*)&data + num_64bit_reads;
        }

  public:
        guard_type front_guard;
        guard_type back_guard;
        U data;
    
        basic_ioguard( const U& data) :
            front_guard(0),
            back_guard(0),
            data(data)
        {
        }

        basic_ioguard() :
            front_guard(0),
            back_guard(0)
        {
        }
    
        U& get_data() {
            return data;
        }

        const U& get_data() const {
            return data;
        }
    
        void init( const U* rhs) {
            ++front_guard;
            architecture_aquire_fence(); // mark *this as unstable
            const volatile uint64_t *End = (const volatile uint64_t *)rhs+num_64bit_reads;
            std::copy( (const volatile uint64_t *)rhs, End, (uint64_t*)&data);
            architecture_release_fence(); // mark *this as stable
            back_guard = front_guard;
        }
   
        void init( const U &rhs) {
            init( &rhs);
        }

        void copy_to_rhs( basic_ioguard* rhs) const {
            ++rhs->front_guard;
            architecture_release_fence(); // mark rhs as unstable
            std::copy( (const volatile uint64_t*)&data, end(), (uint64_t*)&rhs->data);
            architecture_release_fence(); // mark rhs as stable
            rhs->back_guard = rhs->front_guard;
        }

        static void copy_to_rhs( const U& data, basic_ioguard* rhs) {
            ++rhs->front_guard;
            architecture_release_fence(); // mark rhs as unstable
            std::copy( (const volatile uint64_t*)&data, 
                    (const volatile uint64_t*)&data + num_64bit_reads, 
                    (uint64_t*)&rhs->data);
            architecture_release_fence(); // mark rhs as stable
            rhs->back_guard = rhs->front_guard;
        }

        void copy_to_rhs( basic_ioguard& rhs) const {
            copy_to_rhs( &rhs);
        }

        void init_reverse_order( const volatile basic_ioguard* rhs) {
            do {
                back_guard = rhs->back_guard;
                std::copy( (const volatile uint64_t*)&rhs->data, rhs->end(), (uint64_t*)&data);
                architecture_aquire_fence();
                front_guard = rhs->front_guard;
            } while (front_guard != back_guard);
        }

        void init_reverse_order( const volatile basic_ioguard& rhs) {
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
