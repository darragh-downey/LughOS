#include "lugh.h"

uint64_t generate_transaction_id(void) {
    static uint64_t next_id = 1; // Start from 1 to avoid zero ID
    return next_id++;
}