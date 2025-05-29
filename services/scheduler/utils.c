#include "lugh.h"

void queue_init(priority_queue_t* queue) {
    queue->count = 0;
}

int queue_push(priority_queue_t* queue, message_t* msg) {
    if (queue->count >= MAX_QUEUE_SIZE) return -1; // Queue is full
    
    // Insert message in sorted order based on priority
    int i = queue->count++;
    while (i > 0 && queue->messages[i-1].priority > msg->priority) {
        queue->messages[i] = queue->messages[i - 1];
        i--;
    }
    
    queue->messages[i] = *msg; // Insert the new message
    return 0; // Success
}

int queue_pop(priority_queue_t* queue, message_t* msg) {
    if (queue->count == 0) return -1; // Queue is empty
    
    *msg = queue->messages[0]; // Get the highest priority message
    for (int i = 1; i < queue->count; i++) {
        queue->messages[i - 1] = queue->messages[i]; // Shift messages down
    }
    
    queue->count--; // Decrease count
    return 0; // Success
}

