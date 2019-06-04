#include <stdlib.h>
#include <thread>
#include "shared_memory_price_server.h"

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
    guarded_price_datum* sec_data = reinterpret_cast<guarded_price_datum*>(data);

    if (first_time) {
        size_t num_tickers = num_sec_codes / NUM_STOCK_EXCHANGE;
        for (size_t i=0; i<num_tickers; ++i) {
            guarded_price_datum* pnt_nbbo = sec_data + i * NUM_STOCK_EXCHANGE;
            calc_new_market_with_random_price_change( pnt_nbbo, NUM_STOCK_EXCHANGE, 0, 1000000);
        }
    }

    return sec_data;
}
