# LughOS Storage Design

## Purpose

The storage service provides persistent data management with transactional updates and rollback, ensuring reliability for critical infrastructure.

## Features

Transactional Updates: Log operations (e.g., writes, scheduler swaps) with commit/rollback.
Prioritized Messaging: Handle high-priority storage requests (e.g., critical logs) first.
Zero Downtime: Support hot-swapping of storage service with state transfer.
Security: Enforce least privilege for storage access.

## Interface

```c
typedef struct {
    uint64_t txn_id;
    char key[64];
    char value[256];
    int operation; // OP_WRITE, OP_DELETE
} txn_log_entry_t;

int log_transaction(txn_log_entry_t* entry);
int commit_transaction(uint64_t txn_id);
int rollback_transaction(uint64_t txn_id);
```

## Implementation Details

Storage Backend: Simple key-value store using a file (storage.dat).
Transaction Log: Log operations to txn_log.dat for atomicity.
NNG Integration: Receive requests via inproc sockets, prioritizing high-priority messages.
State Transfer: Serialize pending transactions during service updates.

## Workflow

Clients send storage requests (e.g., OP_WRITE) via NNG.
Service logs requests to txn_log.dat.
On commit, apply changes to storage.dat.
On failure, rollback using logged entries.
During service swaps, transfer transaction log to new instance.

## MVP Scope

Implement basic key-value store with transactional logging.
Support high-priority write requests for critical data.
Ensure zero downtime during storage service updates.
