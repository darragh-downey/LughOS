#include "lugh.h"

// JPL Rule 13: Limited scope for data and functions
// SEI CERT DCL30-C: Declare objects with appropriate storage duration
// SEI CERT INT30-C: Ensure unsigned integer operations do not wrap
uint64_t generate_transaction_id(void) {
    static uint64_t next_id = 1; // Start from 1 to avoid zero ID
    // JPL Rule 16: Use assertions for critical invariants (not shown, but should check for wrap)
    return next_id++;
}