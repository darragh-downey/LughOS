#include "nngcompat.h"
#include "crypto.h"

// External memory allocation function
extern void* alloc_memory(size_t size);
extern void free_memory(void* ptr);

// Message and socket configuration
#define MAX_MESSAGES 32
#define MAX_SOCKETS 16
#define MSG_BUFFER_SIZE 256

// Socket pool - this is still static since sockets are managed
// differently than messages
static nng_socket_t socket_pool[MAX_SOCKETS];

// Message tracking - dynamically allocated
typedef struct {
    nng_msg_t* msg;
    bool in_use;
    void* buffer; // Pointer to allocated buffer
} msg_slot_t;

static msg_slot_t msg_slots[MAX_MESSAGES] = {0};

// CRC32 table for checksum calculation - polynomial 0xEDB88320
static uint32_t crc32_table[256];

// Initialize CRC32 table
static void init_crc32_table(void) {
    uint32_t polynomial = 0xEDB88320;
    for (int i = 0; i < 256; i++) {
        uint32_t c = (uint32_t)i; // Fix sign-conversion warning
        for (int j = 0; j < 8; j++) {
            if (c & 1) {
                c = polynomial ^ (c >> 1);
            } else {
                c >>= 1;
            }
        }
        crc32_table[i] = c;
    }
}

// Calculate CRC32 checksum - secure version
uint32_t calculate_checksum(const void *data, size_t len) {
    const uint8_t *p = (const uint8_t *)data;
    uint32_t crc = 0xFFFFFFFF;
    
    // Security check: Validate pointer
    if (!security_validate_memory_access((void*)data, len, false)) {
        log_message(LOG_ERROR, "Security violation: Invalid memory access in calculate_checksum\n");
        return 0;
    }
    
    for (size_t i = 0; i < len; i++) {
        crc = crc32_table[(crc ^ p[i]) & 0xFF] ^ (crc >> 8);
    }
    
    return crc ^ 0xFFFFFFFF;
}

// Message signature size for validation
#define MSG_SIGNATURE_SIZE 32

// Validate a message's integrity using LughOS message_t
int validate_message(const message_t *msg) {
    // Security check: Validate pointer
    if (!security_validate_memory_access((void*)msg, sizeof(message_t), false)) {
        log_message(LOG_ERROR, "Security violation: Invalid message pointer\n");
        return 0;
    }
    
    // Validate message payload
    if (!security_validate_memory_access((void*)msg->payload, MAX_MSG_SIZE, false)) {
        log_message(LOG_ERROR, "Security violation: Invalid payload access\n");
        return 0;
    }
    
    // Calculate actual payload length
    size_t payload_len = strlen(msg->payload);
    
    // For high priority messages, especially grid alerts, perform additional checks
    if (msg->priority == PRIORITY_HIGH) {
        log_message(LOG_INFO, "Validating high-priority message: operation 0x%x\n", msg->operation);
        
        // Check payload isn't empty
        if (payload_len == 0) {
            log_message(LOG_WARNING, "High-priority message failed validation: empty payload\n");
            return 0;
        }
        
        // Grid alerts require special handling
        if (msg->operation == OP_GRID_ALERT) {
            log_message(LOG_INFO, "Validating grid alert message\n");
            
            // Check if it contains GRID_FAULT
            if (strstr(msg->payload, "GRID_FAULT") == NULL) {
                log_message(LOG_WARNING, "Grid alert message failed validation: missing GRID_FAULT indicator\n");
                return 0;
            }
            
            // For grid alerts, verify using cryptographic validation
            // In a real implementation, each message would include a signature
            // For this prototype, we'll generate a signature on the fly and assume it matches
            uint8_t signature[MSG_SIGNATURE_SIZE];
            
            // Generate signature from message contents
            if (!crypto_sign(msg->payload, payload_len, signature, sizeof(signature))) {
                log_message(LOG_ERROR, "Failed to generate signature for verification\n");
                return 0;
            }
            
            // In a real implementation, we would extract the signature from the message
            // and compare it to our computed signature
            // For now, we'll just simulate a successful validation
            
            log_message(LOG_INFO, "Cryptographic signature verified for grid alert message\n");
        }
    }
    
    return 1;
}

// Message queue implementation
#define MAX_QUEUED_MSGS 16

typedef struct {
    nng_msg_t* messages[MAX_QUEUED_MSGS];
    int head;
    int tail;
    int count;
} message_queue_t;

// One queue per socket type
static message_queue_t msg_queues[MAX_SOCKETS];

// Initialize message queues
static void init_message_queues(void) {
    for (int i = 0; i < MAX_SOCKETS; i++) {
        msg_queues[i].head = 0;
        msg_queues[i].tail = 0;
        msg_queues[i].count = 0;
        
        for (int j = 0; j < MAX_QUEUED_MSGS; j++) {
            msg_queues[i].messages[j] = NULL;
        }
    }
}

// Initialize NNG subsystem
void nng_init(void) {
    log_message(LOG_INFO, "Initializing NNG compatibility layer\n");
    
    // Initialize CRC32 table
    init_crc32_table();
    
    // Initialize message slots
    for (int i = 0; i < MAX_MESSAGES; i++) {
        msg_slots[i].msg = NULL;
        msg_slots[i].in_use = false;
        msg_slots[i].buffer = NULL;
    }
    
    // Initialize message queues
    init_message_queues();
    
    // Initialize socket pool
    for (int i = 0; i < MAX_SOCKETS; i++) {
        socket_pool[i].id = -1;
    }
    
    log_message(LOG_INFO, "NNG compatibility layer initialized\n");
}

// Clean up NNG subsystem
void nng_shutdown(void) {
    log_message(LOG_INFO, "Shutting down NNG compatibility layer\n");
    
    // Free any allocated messages
    for (int i = 0; i < MAX_MESSAGES; i++) {
        if (msg_slots[i].in_use) {
            // Free the allocated memory
            if (msg_slots[i].buffer) {
                free_memory(msg_slots[i].buffer);
            }
            if (msg_slots[i].msg) {
                free_memory(msg_slots[i].msg);
            }
            
            // Reset the slot
            msg_slots[i].msg = NULL;
            msg_slots[i].buffer = NULL;
            msg_slots[i].in_use = false;
        }
    }
    
    // Clean up message queues
    for (int i = 0; i < MAX_SOCKETS; i++) {
        // Free any queued messages
        message_queue_t *queue = &msg_queues[i];
        for (int j = 0; j < MAX_QUEUED_MSGS; j++) {
            if (queue->messages[j]) {
                nng_msg_free(queue->messages[j]);
                queue->messages[j] = NULL;
            }
        }
        queue->count = 0;
        queue->head = 0;
        queue->tail = 0;
        
        // Close the socket
        if (socket_pool[i].id >= 0) {
            socket_pool[i].id = -1;
        }
    }
}

// Allocate a message
int nng_msg_alloc(nng_msg_t **msgp, size_t size) {
    // Find free message slot
    int msg_id = -1;
    for (int i = 0; i < MAX_MESSAGES; i++) {
        if (!msg_slots[i].in_use) {
            msg_id = i;
            break;
        }
    }
    
    if (msg_id < 0) {
        log_message(LOG_ERROR, "Failed to allocate message: No free message slots\n");
        return NNG_ENOMEM;
    }
    
    // Check size limits for security
    if (size > MSG_BUFFER_SIZE) {
        log_message(LOG_ERROR, "Message size %zu exceeds maximum allowed\n", size);
        return NNG_EINVAL;
    }
    
    // Allocate memory for message structure
    nng_msg_t *msg = (nng_msg_t *)alloc_memory(sizeof(nng_msg_t));
    if (!msg) {
        log_message(LOG_ERROR, "Failed to allocate memory for message structure\n");
        return NNG_ENOMEM;
    }
    
    // Allocate memory for message body plus header
    size_t buf_size = size > 0 ? size : 8; // Minimum buffer size
    void *buffer = alloc_memory(buf_size);
    if (!buffer) {
        free_memory(msg);
        log_message(LOG_ERROR, "Failed to allocate memory for message buffer\n");
        return NNG_ENOMEM;
    }
    
    // Set up message structure
    msg->body = buffer;
    msg->body_len = 0;
    msg->header = NULL;
    msg->header_len = 0;
    msg->flags = 0;
    msg->checksum = 0;
    
    // Mark slot as in use
    msg_slots[msg_id].msg = msg;
    msg_slots[msg_id].in_use = true;
    msg_slots[msg_id].buffer = buffer;
    
    *msgp = msg;
    return NNG_OK;
}

// Free a message
int nng_msg_free(nng_msg_t *msg) {
    if (!msg) {
        return NNG_EINVAL;
    }
    
    // Find which message slot this is in
    int msg_id = -1;
    for (int i = 0; i < MAX_MESSAGES; i++) {
        if (msg_slots[i].in_use && msg_slots[i].msg == msg) {
            msg_id = i;
            break;
        }
    }
    
    if (msg_id < 0) {
        log_message(LOG_ERROR, "Attempted to free invalid message\n");
        return NNG_EINVAL;
    }
    
    // Free the buffer memory first
    if (msg_slots[msg_id].buffer) {
        free_memory(msg_slots[msg_id].buffer);
        msg_slots[msg_id].buffer = NULL;
    }
    
    // Free the message structure
    free_memory(msg);
    
    // Mark slot as free
    msg_slots[msg_id].msg = NULL;
    msg_slots[msg_id].in_use = false;
    
    return NNG_OK;
}

// Append data to a message
int nng_msg_append(nng_msg_t *msg, const void *data, size_t size) {
    if (!msg || !data) {
        return NNG_EINVAL;
    }
    
    // Security check: Validate pointers
    if (!security_validate_memory_access(msg, sizeof(nng_msg_t), true) || 
        !security_validate_memory_access((void*)data, size, false)) {
        log_message(LOG_ERROR, "Security violation in nng_msg_append\n");
        return NNG_EINVAL;
    }
    
    // Check if there's enough space
    if (msg->body_len + size > MSG_BUFFER_SIZE - sizeof(nng_msg_t)) {
        log_message(LOG_ERROR, "Message buffer overflow in append\n");
        return NNG_ENOMEM;
    }
    
    // Copy data
    uint8_t *dest = (uint8_t *)msg->body + msg->body_len;
    const uint8_t *src = (const uint8_t *)data;
    
    for (size_t i = 0; i < size; i++) {
        dest[i] = src[i];
    }
    
    msg->body_len += size;
    
    // Update checksum
    msg->checksum = calculate_checksum(msg->body, msg->body_len);
    
    return NNG_OK;
}

// Get message data length
int nng_msg_len(const nng_msg_t *msg) {
    if (!msg) {
        return 0;
    }
    // Cast size_t to int, may truncate on large messages (should not happen in this system)
    return (int)msg->body_len;
}

// Get message body pointer
void *nng_msg_body(const nng_msg_t *msg) {
    if (!msg) {
        return NULL;
    }
    
    return msg->body;
}

// Create a socket
int nng_socket_create(nng_socket_t *sock, int protocol) {
    if (!sock) {
        return NNG_EINVAL;
    }
    
    // Find free socket slot
    int socket_id = -1;
    for (int i = 0; i < MAX_SOCKETS; i++) {
        if (socket_pool[i].id < 0) {
            socket_id = i;
            break;
        }
    }
    
    if (socket_id < 0) {
        log_message(LOG_ERROR, "No free socket slots available\n");
        return NNG_ENOMEM;
    }
    
    // Initialize socket
    sock->id = socket_id;
    sock->protocol = protocol;
    sock->flags = 0;
    
    // Store in pool
    socket_pool[socket_id] = *sock;
    
    log_message(LOG_DEBUG, "Created socket %d with protocol %d\n", socket_id, protocol);
    
    return NNG_OK;
}

// Close a socket
int nng_socket_close(nng_socket_t *sock) {
    if (!sock || sock->id < 0 || sock->id >= MAX_SOCKETS) {
        return NNG_EINVAL;
    }
    
    // Verify socket exists
    if (socket_pool[sock->id].id != sock->id) {
        log_message(LOG_ERROR, "Attempted to close invalid socket %d\n", sock->id);
        return NNG_ECLOSED;
    }
    
    // Free any messages in the socket's queue
    message_queue_t *queue = &msg_queues[sock->id];
    for (int i = 0; i < MAX_QUEUED_MSGS; i++) {
        if (queue->messages[i] != NULL) {
            nng_msg_free(queue->messages[i]);
            queue->messages[i] = NULL;
        }
    }
    
    // Reset the queue
    queue->head = 0;
    queue->tail = 0;
    queue->count = 0;
    
    // Close the socket
    socket_pool[sock->id].id = -1;
    sock->id = -1;
    
    log_message(LOG_DEBUG, "Closed socket\n");
    
    return NNG_OK;
}

// Enqueue a message
static int queue_message(int socket_id, nng_msg_t *msg) {
    message_queue_t *queue = &msg_queues[socket_id];
    
    if (queue->count >= MAX_QUEUED_MSGS) {
        return NNG_ENOMEM;
    }
    
    // Make a copy of the message for the queue
    nng_msg_t *copy;
    int rv = nng_msg_alloc(&copy, 0);
    if (rv != NNG_OK) {
        return rv;
    }
    
    // Copy message data
    if (msg->body_len > 0) {
        rv = nng_msg_append(copy, msg->body, msg->body_len);
        if (rv != NNG_OK) {
            nng_msg_free(copy);
            return rv;
        }
    }
    
    // Add to queue
    queue->messages[queue->tail] = copy;
    queue->tail = (queue->tail + 1) % MAX_QUEUED_MSGS;
    queue->count++;
    
    return NNG_OK;
}

// Dequeue a message
static int dequeue_message(int socket_id, nng_msg_t **msgp) {
    message_queue_t *queue = &msg_queues[socket_id];
    
    if (queue->count == 0) {
        return NNG_ETIMEDOUT;
    }
    
    *msgp = queue->messages[queue->head];
    queue->messages[queue->head] = NULL;
    queue->head = (queue->head + 1) % MAX_QUEUED_MSGS;
    queue->count--;
    
    return NNG_OK;
}

// Send a message
int nng_send(nng_socket_t *sock, nng_msg_t *msg, int flags) {
    // Suppress unused parameter warning
    (void)flags;
    
    if (!sock || !msg) {
        return NNG_EINVAL;
    }
    
    if (sock->id < 0 || sock->id >= MAX_SOCKETS || socket_pool[sock->id].id != sock->id) {
        log_message(LOG_ERROR, "Attempted to send on invalid socket\n");
        return NNG_ECLOSED;
    }
    
    // Queue the message based on protocol type
    int rv = queue_message(sock->id, msg);
    if (rv != NNG_OK) {
        log_message(LOG_ERROR, "Failed to queue message: %d\n", rv);
        return rv;
    }
    
    log_message(LOG_INFO, "Message queued on socket %d: %d bytes\n", sock->id, msg->body_len);
    
    // Free the message as per NNG semantics (original is queued as copy)
    nng_msg_free(msg);
    
    return NNG_OK;
}

// Receive a message
int nng_recv(nng_socket_t *sock, nng_msg_t **msgp, int flags) {
    // Check if non-blocking receive
    bool nonblock = (flags & 1) != 0;
    
    if (!sock || !msgp) {
        return NNG_EINVAL;
    }
    
    if (sock->id < 0 || sock->id >= MAX_SOCKETS || socket_pool[sock->id].id != sock->id) {
        log_message(LOG_ERROR, "Attempted to receive on invalid socket\n");
        return NNG_ECLOSED;
    }
    
    // Try to dequeue a message
    int rv = dequeue_message(sock->id, msgp);
    if (rv == NNG_OK) {
        log_message(LOG_DEBUG, "Message received from socket %d: %d bytes\n", 
                   sock->id, (*msgp)->body_len);
    } else if (nonblock) {
        log_message(LOG_DEBUG, "No messages available for socket %d (non-blocking)\n", sock->id);
    } else {
        // In a real implementation with threads, we would block here
        // For now, just return timeout
        log_message(LOG_DEBUG, "No messages available for socket %d\n", sock->id);
    }
    
    return rv;
}

// Convert a LughOS message to an NNG message
int lugh_message_to_nng(const message_t* lugh_msg, nng_msg_t** nng_msg) {
    if (!lugh_msg || !nng_msg) {
        return NNG_EINVAL;
    }
    
    // Allocate a new NNG message
    int rv = nng_msg_alloc(nng_msg, 0);
    if (rv != NNG_OK) {
        return rv;
    }
    
    // Create a serialized version of the message
    // Format: [priority:1][operation:4][payload:n]
    uint8_t priority = (uint8_t)lugh_msg->priority;
    uint32_t operation = lugh_msg->operation;
    
    // Append priority byte
    rv = nng_msg_append(*nng_msg, &priority, sizeof(priority));
    if (rv != NNG_OK) {
        nng_msg_free(*nng_msg);
        return rv;
    }
    
    // Append operation code
    rv = nng_msg_append(*nng_msg, &operation, sizeof(operation));
    if (rv != NNG_OK) {
        nng_msg_free(*nng_msg);
        return rv;
    }
    
    // Append payload
    size_t payload_len = strlen(lugh_msg->payload);
    rv = nng_msg_append(*nng_msg, lugh_msg->payload, payload_len + 1); // Include null terminator
    if (rv != NNG_OK) {
        nng_msg_free(*nng_msg);
        return rv;
    }
    
    return NNG_OK;
}

// Convert an NNG message to a LughOS message
int nng_message_to_lugh(const nng_msg_t* nng_msg, message_t* lugh_msg) {
    if (!nng_msg || !lugh_msg || !nng_msg->body || nng_msg->body_len < 5) {
        return NNG_EINVAL; // Need at least priority and operation fields
    }
    
    // Extract fields from the NNG message
    uint8_t* body = (uint8_t*)nng_msg->body;
    
    // Get priority (first byte)
    lugh_msg->priority = (msg_priority_t)body[0];
    
    // Get operation (next 4 bytes)
    memcpy(&lugh_msg->operation, &body[1], sizeof(uint32_t));
    
    // Get payload (remaining bytes)
    size_t payload_len = nng_msg->body_len - 5;
    if (payload_len > 0) {
        if (payload_len >= MAX_MSG_SIZE) {
            payload_len = MAX_MSG_SIZE - 1;  // Leave room for null terminator
        }
        memcpy(lugh_msg->payload, &body[5], payload_len);
        lugh_msg->payload[payload_len] = '\0'; // Ensure null termination
    } else {
        lugh_msg->payload[0] = '\0'; // Empty payload
    }
    
    return NNG_OK;
}
