#ifndef ARCHITECTURE_H
#define ARCHITECTURE_H

//https://stackoverflow.com/questions/152016/detecting-cpu-architecture-compile-time

#if defined(_MSC_VER)
#define ARCH_X86
#elif defined(__GNUC__) || defined( __ICL) 
#define ARCH_X86
#endif

#if defined(ARCH_X86)
#define CACHE_LINE_SIZE 64
#endif

#endif
