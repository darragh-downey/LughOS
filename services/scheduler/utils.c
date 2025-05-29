#include "lugh.h"

void queue_init(priority_queue_t* queue) {
    // SEI CERT ARR30-C: Validate pointer before use
    if (queue == NULL) {
        log_message(LOG_ERROR, "Security violation: NULL pointer in queue_init\n");
        return;
    }
    queue->count = 0;
}

int queue_push(priority_queue_t* queue, message_t* msg) {
    // SEI CERT ARR30-C: Validate pointers before use
    if (queue == NULL || msg == NULL) {
        log_message(LOG_ERROR, "Security violation: NULL pointer in queue_push\n");
        return -1;
    }
    // NASA Power of Ten Rule 2: Bounded queue size
    if (queue->count >= MAX_QUEUE_SIZE) {
        log_message(LOG_WARNING, "Queue overflow prevented: %d items\n", queue->count);
        return -1;
    }
    // SEI CERT INT32-C: Prevent integer overflow
    if (queue->count == INT32_MAX) {
        log_message(LOG_ERROR, "Integer overflow prevented in queue_push\n");
        return -1;
    }
    // NASA Power of Ten Rule 6: Data integrity via checksum
    msg->checksum = calculate_checksum(msg->payload, strlen(msg->payload));
    // NASA Power of Ten Rule 2: Bounded insertion loop
    int i = queue->count++;
    while (i > 0 && queue->messages[i-1].priority > msg->priority) {
        queue->messages[i] = queue->messages[i - 1];
        i--;
        // NASA Power of Ten Rule 4: Small function, bounded loop
    }
    queue->messages[i] = *msg;
    // NASA Power of Ten Rule 6: Verify checksum after insertion
    uint32_t verify_sum = calculate_checksum(queue->messages[i].payload, strlen(queue->messages[i].payload));
    if (verify_sum != queue->messages[i].checksum) {
        log_message(LOG_ERROR, "Message checksum failed after insertion\n");
        return -2;
    }
    return 0;
}

int queue_pop(priority_queue_t* queue, message_t* msg) {
    // SEI CERT ARR30-C: Validate pointers before use
    if (queue == NULL || msg == NULL) {
        log_message(LOG_ERROR, "Security violation: NULL pointer in queue_pop\n");
        return -1;
    }
    if (queue->count == 0) {
        return -1;
    }
    *msg = queue->messages[0];
    // NASA Power of Ten Rule 6: Verify checksum during retrieval
    uint32_t verify_sum = calculate_checksum(msg->payload, strlen(msg->payload));
    if (verify_sum != msg->checksum) {
        log_message(LOG_ERROR, "Message checksum failed during retrieval\n");
        return -2;
    }
    // NASA Power of Ten Rule 2: Bounded loop for shifting messages
    for (int i = 1; i < queue->count && i < MAX_QUEUE_SIZE; i++) {
        queue->messages[i - 1] = queue->messages[i];
    }
    queue->count--;
    return 0;
}

