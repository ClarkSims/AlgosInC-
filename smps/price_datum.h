#ifndef PRICE_DATUM_H
#define PRICE_DATUM_H

#include "IOGuard.h"

struct price_datum {
    uint64_t time;
    uint32_t bid;
    uint32_t bid_size;
    uint32_t ask;
    uint32_t ask_size;
};

typedef util_ipc::IOGuard<price_datum> guarded_price_datum;

#endif
