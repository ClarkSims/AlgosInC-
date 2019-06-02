#ifndef ARCHITECTURE_H
#define ARCHITECTURE_H

//https://stackoverflow.com/questions/152016/detecting-cpu-architecture-compile-time

#ifdef MSVC
#ifdef _M_X86
#define ARCH_X86
#endif
#endif

#ifdef GCC
#ifdef __i386__
#define ARCH_X86
#endif
#endif

#endif
