#ifndef SECURITY_ENCODING_H
#define SECURITY_ENCODING_H

#include <assert.h>
#include <string.h>

struct security_encoding {
    char ticker[8];
    char exchange[8];
    uint32_t id;
    uint32_t denominator;
    char padding[8];

    security_encoding() {
        memset( this, 0, sizeof(*this));
    }

    void init(
       const char *Ticker,
        const char *Exchange,
        uint32_t Id,
        uint32_t Denom)
    {
        assert( strlen(Ticker) < sizeof(ticker));
        assert( strlen(Exchange) < sizeof(exchange));
        strcpy( ticker, Ticker);
        strcpy( exchange, Exchange);
        id = Id;
        denominator = Denom;
    }

    security_encoding(
        const char *Ticker,
        const char *Exchange,
        uint32_t Id=0,
        uint32_t Denom=100):
        id(Id),
        denominator(Denom)
    {
        assert( strlen(Ticker) < sizeof(ticker));
        assert( strlen(Exchange) < sizeof(exchange));
        strcpy( ticker, Ticker);
        strcpy( exchange, Exchange);
    }
};
#endif
