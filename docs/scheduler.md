# LughOS Scheduler Design

## Purpose

The scheduler service manages task execution in LughOS, supporting hot-swapping with zero downtime and prioritized messaging for critical infrastructure.

## Features

Hot-Swappable Schedulers: Swap between algorithms (e.g., round-robin, priority-based) without interrupting tasks.
Prioritized Messaging: Process high-priority scheduling commands first (e.g., for real-time tasks).
Zero Downtime: Use Tanenbaum’s live update mechanism for state transfer and message buffering.
Transactional Updates: Log swaps for rollback if failures occur.

## Interface

```c
typedef struct {
    const char* name;
    int (*init)(void* context);
    int (*schedule)(task_t* tasks, int num_tasks, uint32_t* next_task_id);
    int (*add_task)(task_t* task);
    int (*remove_task)(uint32_t task_id);
    int (*get_state)(void* state_buffer, size_t* size);
    int (*set_state)(void* state_buffer, size_t size);
    int (*prepare_swap)(void);
    int (*finalize_swap)(void);
} scheduler_ops_t;
```

## Implementation Details

Round-Robin Scheduler: Cycles through ready tasks for fair scheduling.
Priority-Based Scheduler: Prioritizes tasks based on priority field.
State Transfer: Serialize task queue and priorities during swaps.
Message Handling: Use NNG inproc sockets with a priority queue for messages.
Transaction Logging: Log swaps to txn_log.dat for rollback.

## Workflow

Kernel sends task management requests (e.g., OP_ADD_TASK, OP_SCHEDULE) to scheduler via NNG.
Scheduler processes high-priority messages first using a priority queue.
During swaps, the reincarnation server:
Calls prepare_swap to pause the old scheduler.
Transfers state (get_state, set_state).
Redirects NNG messages to the new scheduler.
Calls finalize_swap to resume scheduling.

## MVP Scope

Implement round-robin scheduler with swap to priority-based scheduler.
Support high-priority messages for scheduling commands.
Ensure zero downtime during swaps with message buffering.
