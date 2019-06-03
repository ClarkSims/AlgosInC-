#include "shared_memory_price_server.h"

volatile uint64_t *init_heartbeats() { 
    void *data = get_shared_memory_object(SECURITY_ENCODING_FILE_NAME, SECURITY_ENCODING_HEARTBEATS_ID, 64);
    volatile uint64_t* rv;
    rv = reinterpret_cast<volatile uint64_t *>(data);
    return rv;
}
