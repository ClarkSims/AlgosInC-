/// \file memory_fence.h
/// \brief wrapper for memory fences to help make them portable

/***************************************************************************
 *   Copyright (C) 2014 by Clark Sims                                      *
 *   http://AcumenSoftwareInc.com/WhoWeAre/Clark_Sims.html                 *
 *   ClarkSims@AcumenSoftwareInc.com                                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef memory_fence_h
#define memory_fence_h

#ifdef _MSC_VER
#include <intrin.h>
#pragma intrinsic(_ReadWriteBarrier)
#pragma intrinsic(_WriteBarrier)
#pragma intrinsic(_ReadBarrier)
#endif

namespace memory_fence {
  inline void compiler_fence() {
#if defined(_MSC_VER)    
    _ReadWriteBarrier();
#elif defined(__GNUC__) 
    __asm__ __volatile__ ("" ::: "memory");
#elif defined( __ICL)
    __memory_barrier();
#endif
  }

  inline void lfence() {
#if defined(_MSC_VER)
    _ReadWriteBarrier();
    __asm { lfence }
#elif defined(__GNUC__) || defined( __ICL)
    __asm__ __volatile__ ("lfence" ::: "memory");
#else
    #error Unsupported Compiler
#endif
  }

  inline void sfence() {
#if defined(_MSC_VER)
    _ReadWriteBarrier();
    __asm { sfence }
#elif defined(__GNUC__) || defined( __ICL)
    __asm__ __volatile__ ("sfence" ::: "memory");
#else
    #error Unsupported Compiler
#endif
  }

  inline void mfence() {
#if defined(_MSC_VER)
    _ReadWriteBarrier();
    __asm { mfence }
#elif defined(__GNUC__) || defined( __ICL)
    __asm__ __volatile__ ("mfence" ::: "memory");
#else
    #error Unsupported Compiler
#endif
  }
}

#endif
