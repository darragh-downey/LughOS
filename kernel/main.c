#include "lugh.h"
#include "security.h"
#include "hardware.h"
#include "nngcompat.h"

extern scheduler_ops_t rr_scheduler;

// Test NNG messaging functionality
void test_nng(void) {
    log_message(LOG_INFO, "Testing NNG messaging functionality...\n");
    // JPL Rule 14: Check return values of all non-void functions
    // SEI CERT EXP12-C: Do not ignore values returned by functions
    // JPL Rule 16: Use assertions for critical invariants (not shown here, but should be used in real tests)
    
    // Create a socket
    nng_socket_t socket;
    int rv = nng_socket_create(&socket, NNG_PROTO_PUB0);
    if (rv != NNG_OK) {
        log_message(LOG_ERROR, "Failed to create socket: %d\n", rv);
        return;
    }
    
    // Test raw NNG messaging first
    nng_msg_t *raw_msg;
    rv = nng_msg_alloc(&raw_msg, 0);
    if (rv != NNG_OK) {
        log_message(LOG_ERROR, "Failed to allocate message: %d\n", rv);
        nng_socket_close(&socket);
        return;
    }
    
    // Add data to message
    const char *data = "Hello from LughOS!";
    // JPL Rule 15: Validate parameters before use
    // SEI CERT STR31-C: Guarantee storage for strings has space for null terminator
    rv = nng_msg_append(raw_msg, data, strlen(data));
    if (rv != NNG_OK) {
        log_message(LOG_ERROR, "Failed to append data to message: %d\n", rv);
        nng_msg_free(raw_msg);
        nng_socket_close(&socket);
        return;
    }
    
    // Send the raw message
    rv = nng_send(&socket, raw_msg, 0);
    if (rv != NNG_OK) {
        log_message(LOG_ERROR, "Failed to send message: %d\n", rv);
        nng_socket_close(&socket);
        return;
    }
    
    log_message(LOG_INFO, "Raw NNG test message sent successfully!\n");
    
    // Now test the LughOS message format conversion
    message_t lugh_msg;
    lugh_msg.priority = PRIORITY_HIGH;
    lugh_msg.operation = OP_GRID_ALERT;
    // SEI CERT STR31-C: Use safe string copy (ensure null-termination)
    strcpy(lugh_msg.payload, "GRID_ALERT: Testing NNG conversion");
    
    // Convert LughOS message to NNG message
    nng_msg_t *converted_msg;
    rv = lugh_message_to_nng(&lugh_msg, &converted_msg);
    if (rv != NNG_OK) {
        log_message(LOG_ERROR, "Failed to convert LughOS message to NNG: %d\n", rv);
        nng_socket_close(&socket);
        return;
    }
    
    // Send the converted message
    rv = nng_send(&socket, converted_msg, 0);
    if (rv != NNG_OK) {
        log_message(LOG_ERROR, "Failed to send converted message: %d\n", rv);
        nng_socket_close(&socket);
        return;
    }
    
    log_message(LOG_INFO, "Converted LughOS->NNG message sent successfully!\n");
    nng_socket_close(&socket);
}

// Forward declaration
void crypto_init(void);

// Test secure messaging for energy sector operations
void test_energy_grid_alert(void) {
    log_message(LOG_INFO, "Testing energy grid alert messaging...\n");
    
    // Create an energy grid alert message
    message_t alert;
    alert.priority = PRIORITY_HIGH;
    alert.operation = OP_GRID_ALERT;
    
    // Copy payload safely
    const char* payload = "GRID_FAULT: Voltage spike detected";
    size_t payload_len = strlen(payload);
    // JPL Rule 15: Validate parameters before use
    // SEI CERT ARR30-C: Validate all array indices
    if (payload_len >= MAX_MSG_SIZE) {
        log_message(LOG_ERROR, "Payload too large for message buffer\n");
        return;
    }
    // SEI CERT STR31-C: Guarantee storage for strings has space for null terminator
    memcpy(alert.payload, payload, payload_len + 1); // +1 for null terminator
    
    // Validate the message with our updated security validation
    log_message(LOG_INFO, "Grid Alert - Priority: %d, Operation: 0x%x, Payload: %s\n", 
               alert.priority, alert.operation, alert.payload);
    
    if (validate_message(&alert)) {
        log_message(LOG_INFO, "Grid alert message passed validation\n");
        
        // Test secure message transfer via NNG
        nng_socket_t socket;
        int rv = nng_socket_create(&socket, NNG_PROTO_PUB0);
        if (rv != NNG_OK) {
            log_message(LOG_ERROR, "Failed to create socket for grid alert: %d\n", rv);
            return;
        }
        
        // Convert to NNG message
        nng_msg_t *msg;
        rv = lugh_message_to_nng(&alert, &msg);
        if (rv != NNG_OK) {
            log_message(LOG_ERROR, "Failed to convert grid alert to NNG: %d\n", rv);
            nng_socket_close(&socket);
            return;
        }
        
        // Send the message
        rv = nng_send(&socket, msg, 0);
        if (rv != NNG_OK) {
            log_message(LOG_ERROR, "Failed to send grid alert: %d\n", rv);
            nng_socket_close(&socket);
            return;
        }
        
        log_message(LOG_INFO, "Grid alert sent successfully\n");
        
        // Try to receive the message back
        nng_msg_t *recv_msg;
        rv = nng_recv(&socket, &recv_msg, 0);
        if (rv == NNG_OK) {
            // Convert back to LughOS message
            message_t recv_alert;
            rv = nng_message_to_lugh(recv_msg, &recv_alert);
            if (rv == NNG_OK) {
                log_message(LOG_INFO, "Received grid alert: %s\n", recv_alert.payload);
            }
            nng_msg_free(recv_msg);
        } else {
            log_message(LOG_INFO, "No grid alerts in queue (expected for PUB socket)\n");
        }
        
        nng_socket_close(&socket);
    } else {
        log_message(LOG_ERROR, "Grid alert message failed validation\n");
    }
}

// Forward declaration of memory_init
void memory_init(void);

void kmain(void) {
    // Initialize essential kernel subsystems
    log_message(LOG_INFO, "%s v%s booting...\n", OS_NAME, OS_VERSION);
    
    // Initialize hardware detection and security features
    if (!hw_detect()) {
        log_message(LOG_ERROR, "Hardware detection failed, halting system\n");
        return;
    }
    
    // Initialize security subsystem first (needed for validations)
    security_init();
    
    // Initialize memory subsystem (Per NASA Power of Ten rule 3: all allocation at init time)
    memory_init();
    
    // Initialize crypto subsystem (depends on memory and security)
    crypto_init();
    
    // Initialize NNG compatibility layer
    nng_init();
    
    // Run NNG tests
    test_nng();
    test_energy_grid_alert();
    
    // Simple microkernel main loop - for now, just idle
    log_message(LOG_INFO, "Entering kernel main loop\n");
    
    while (1) {
        // Process any pending events
        process_events();
        
        // Idle the CPU to conserve power
        cpu_idle();
    }
}