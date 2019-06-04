#include <iostream>
#include "price_datum.h"
#include "security_encoding.h"

void price_datum::print( security_encoding* sec_code)
{
    double dbid, dask;
    dbid = (double)bid / sec_code->denominator;
    dask = (double)ask / sec_code->denominator;
    std::cout << time << " "
            << sec_code->exchange << " "
            << sec_code->ticker << " "
            << dbid << " "
            << dask << " "
            << bid_size << " "
            << ask_size << std::endl;
}

