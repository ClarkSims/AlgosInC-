#ifndef IOGUARD_H
#define IOGUARD_H

#include "basic_ioguard.h"

namespace util_ipc {
  template <class W>
    using IOGuard = basic_ioguard<W,uint32_t,sizeof(W)/sizeof(uint64_t)>;
}

#endif
