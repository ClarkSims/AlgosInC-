#include <stdlib.h>
#include <iostream>
#include "shared_memory_price_server.h"

void print_snapshot(
    guarded_price_datum* snapshot,
    security_encoding* sec_codes,
    size_t num_sec_codes)
{
    price_datum pd;
    for (size_t off=0; off<num_sec_codes; ++off)
    {
        if (snapshot[off].try_copy_to_rhs_reverse_order( &pd, 100))
        {
            pd.print(sec_codes + off);
        }
        else
        {
            std::cerr << "price data locked for "
                      << sec_codes[off].exchange 
                      << " " 
                      << sec_codes[off].ticker << std::endl;
        }
    }
}        

guarded_price_datum* init_snapshot(
    security_encoding* sec_codes,
    size_t num_sec_codes,
    bool first_time) 
{
    size_t buf_size = num_sec_codes * NUM_STOCK_EXCHANGE * sizeof(guarded_price_datum);
    void *data = get_shared_memory_object(
        SECURITY_ENCODING_FILE_NAME,
        SECURITY_SNAPSHOT_SHMEM_ID,
        buf_size);
    guarded_price_datum* snapshot = reinterpret_cast<guarded_price_datum*>(data);

    if (first_time) {
        size_t num_tickers = num_sec_codes / NUM_STOCK_EXCHANGE;
        for (size_t i=0; i<num_tickers; ++i) {
            guarded_price_datum* pnt_nbbo = snapshot + i * NUM_STOCK_EXCHANGE;
            calc_new_market_with_random_price_change( pnt_nbbo, NUM_STOCK_EXCHANGE, 0, 1000000);
        }
    }

    return snapshot;
}
