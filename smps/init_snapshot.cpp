#include <stdlib.h>
#include <thread>
#include <vector>
#include "shared_memory_price_server.h"

/*
struct price_datum {
    uint64_t time;
    uint32_t bid;
    uint32_t bid_size;
    uint32_t ask;
    uint32_t ask_size;
};
*/

void calc_new_market(
    guarded_price_datum* pnt_nbbo,
    int      num_venue,
    uint64_t time,
    uint32_t midpoint)
{
    int32_t d_mid = rand() % 11 - 5;
    midpoint += d_mid;
    price_datum pd, nbbo_pd;
    pd.time = time;
    
    for (int i = 1; i<num_venue; ++i) {
        pd.bid = midpoint - rand() % 10;
        pd.ask = midpoint + rand() % 10;
        pd.bid_size = rand() % 10 + 1;
        pd.ask_size = rand() % 10 + 1;
        guarded_price_datum::copy_to_rhs( pd, pnt_nbbo + i);

        if (i == 1) {
            nbbo_pd = pd;
        } else {
            if (pd.bid > nbbo_pd.bid) {
                nbbo_pd.bid = pd.bid;
                nbbo_pd.bid_size = pd.bid_size;
            }
            if (pd.ask < nbbo_pd.ask) {
                nbbo_pd.ask = pd.ask;
                nbbo_pd.ask_size = pd.ask_size;
            }
        }
    }
    guarded_price_datum::copy_to_rhs( nbbo_pd, pnt_nbbo);
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
    guarded_price_datum* sec_data = reinterpret_cast<guarded_price_datum*>(data);

    if (first_time) {
        size_t num_tickers = num_sec_codes / NUM_STOCK_EXCHANGE;
        for (size_t i=0; i<num_tickers; ++i) {
            guarded_price_datum* pnt_nbbo = sec_data + i * NUM_STOCK_EXCHANGE;
            calc_new_market( pnt_nbbo, NUM_STOCK_EXCHANGE, 0, 1000000);
        }
    }

    return sec_data;
}
