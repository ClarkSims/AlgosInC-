/// \file memory_fence.h
/// \brief wrapper for memory fences to help make them portable

#ifndef memory_fence_h
#define memory_fence_h

#if ARCH_X86
namespace memory_fence {

  inline void sfence() {
#if defined(_MSC_VER)
    atomic_signal_fence(memory_order_acq_rel);
    __asm { sfence }
    atomic_signal_fence(memory_order_acq_rel);
#elif defined(__GNUC__) || defined( __ICL)
    __asm__ __volatile__ ("sfence" ::: "memory");
#else
    #error Unsupported Compiler
#endif
  }

  inline void mfence() {
#if defined(_MSC_VER)
    atomic_signal_fence(memory_order_acq_rel);
    __asm { mfence }
    atomic_signal_fence(memory_order_acq_rel);
#elif defined(__GNUC__) || defined( __ICL)
    __asm__ __volatile__ ("mfence" ::: "memory");
#else
    #error Unsupported Compiler
#endif
  }
}
#endif

#endif
