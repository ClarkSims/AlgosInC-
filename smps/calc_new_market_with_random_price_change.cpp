#include "shared_memory_price_server.h"

void calc_new_market_with_random_price_change(
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
