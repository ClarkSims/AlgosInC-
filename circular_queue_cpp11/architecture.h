#ifndef ARCHITECTURE_H
#define ARCHITECTURE_H

//https://stackoverflow.com/questions/152016/detecting-cpu-architecture-compile-time

#if defined(_MSC_VER)
#define ARCH_X86
#elif defined(__GNUC__) || defined( __ICL) 
#define ARCH_X86
#endif // _MSC_VER

#if defined(ARCH_X86)
#define CACHE_LINE_SIZE 64
#endif // ARCH_X86

#include "memory_fence.h"

#if defined(ARCH_X86)
#define architecture_aquire_fence()  std::atomic_signal_fence(std::memory_order_acq_rel)
#define architecture_release_fence() memory_fence::sfence()
// define pause
#if defined(_MSC_VER)
#define architecture_pause()  _ReadWriteBarrier(), __asm { pause }, _ReadWriteBarrier()
#elif defined(__GNUC__) || defined( __ICL)
#define architecture_pause()  __asm__ __volatile__ ("pause" ::: "memory");
#endif // _MSC_VER
#else // ARCH_X86
#define architecture_aquire_fence()  std::atomic_thread_fence(std::memory_order_acquire)
#define architecture_release_fence() std::atomic_thread_fence(std::memory_order_release)
#define architecture_pause()
#endif // ARCH_X86

#endif
