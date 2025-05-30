#ifndef UPDATE_H
#define UPDATE_H

/**
 * Update System Header for LughOS
 *
 * Defines the interfaces for the transactional update system
 * that enables secure, atomic updates with rollback capability.
 *
 * Complies with:
 * - SEI CERT STR31-C: Guarantee null-terminated strings
 * - JPL Rule 6: Restrict length of data blocks
 * - NASA Rule 1: Simple control flow
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Update types */
typedef enum {
    UPDATE_TYPE_DRIVER,
    UPDATE_TYPE_SERVICE,
    UPDATE_TYPE_KERNEL,
    UPDATE_TYPE_USER
} update_type_t;

/* Update status */
typedef enum {
    UPDATE_STATUS_INIT,
    UPDATE_STATUS_CHECKPOINT,
    UPDATE_STATUS_VERIFY,
    UPDATE_STATUS_SANDBOX,
    UPDATE_STATUS_TEST,
    UPDATE_STATUS_COMMIT,
    UPDATE_STATUS_COMPLETE,
    UPDATE_STATUS_ROLLBACK,
    UPDATE_STATUS_ERROR
} update_status_t;

/* Base transaction structure - same as in sandbox.c */
struct update_tx {
    char *path;         /* Update file path */
    char *checkpoint;   /* Backup path */
    uint8_t *image;     /* Update binary */
    size_t size;
    uint32_t hash;      /* SHA-256; SEI CERT MSC41-C */
};

/* Extended update transaction structure */
struct update_state {
    struct update_tx tx;           /* Base transaction data */
    update_type_t type;            /* Type of update */
    update_status_t status;        /* Current status */
    uint64_t transaction_id;       /* Unique ID for logging */
    char log_path[256];            /* Path to transaction log */
    uint32_t error_count;          /* Number of errors encountered */
    bool requires_reboot;          /* Whether update requires reboot */
};

/**
 * Initialize an update transaction structure.
 * 
 * @param state Update state structure to initialize
 * @param type Type of update
 * @param path Target path for the update
 * @param image Binary image to apply
 * @param size Size of the binary image
 * @param hash Expected hash for verification
 * @return 0 on success, -1 on failure
 */
int init_update_transaction(struct update_state *state, update_type_t type, 
                          const char *path, const uint8_t *image, size_t size, 
                          uint32_t hash);

/**
 * Execute a complete update transaction with proper error handling.
 * 
 * @param state Update state structure
 * @return 0 on success, -1 on failure
 */
int execute_update(struct update_state *state);

/**
 * Clean up an update transaction, freeing any resources.
 * 
 * @param state Update state to clean up
 */
void cleanup_update_transaction(struct update_state *state);

/**
 * Apply an update with proper validation and rollback capability.
 * 
 * @param tx Basic transaction structure
 * @return 0 on success, -1 on failure
 */
int apply_update(struct update_tx *tx);

/**
 * Process IPC message for update command.
 * 
 * @param operation Operation code from IPC message
 * @param message Message payload containing update info
 * @return 0 on success, -1 on failure
 */
int process_update_ipc(uint32_t operation, const char *message);

/**
 * Roll back an update if it fails.
 * 
 * @param tx Update transaction to roll back
 */
void rollback_update(struct update_tx *tx);

/**
 * Commit an update after successful validation.
 * 
 * @param tx Update transaction to commit
 */
void commit_update(struct update_tx *tx);

#endif /* UPDATE_H */
