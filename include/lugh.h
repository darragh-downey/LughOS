#ifndef LUGHOS_H
#define LUGHOS_H

#include <nng/nng.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdarg.h>

/* System constants */
#define OS_NAME "LughOS"
#define OS_VERSION "0.0.1"
#define OS_AUTHOR "Darragh Downey"
#define MAX_TASKS 1024
#define MAX_MSG_SIZE 128
#define MAX_QUEUE_SIZE 1024
#define MAX_OPERATIONS 0x200

/* Architecture-specific constants */
#ifdef __i386__
#define KERNEL_PAGE_SIZE 4096
#elif defined(__arm__)
#define KERNEL_PAGE_SIZE 4096 /* Adjust for ARM if needed */
#else
#define KERNEL_PAGE_SIZE 4096
#endif

/* Task states */
#define TASK_READY 0
#define TASK_RUNNING 1
#define TASK_BLOCKED 2
#define TASK_TERMINATED 3

/* Message operations */
#define OP_ADD_TASK 0x01
#define OP_SCHEDULE 0x02
#define OP_GRID_ALERT 0x100 /* Critical infrastructure: energy grid fault */
#define OP_HEARTBEAT 0x101  /* Future: distributed operation */
#define OP_WRITE 0x200
#define OP_DELETE 0x201

/* Logging levels */
typedef enum {
    LOG_DEBUG = 0,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_FATAL,
    LOG_LEVEL_COUNT
} log_level_t;

/* Task structure for scheduler */
typedef struct {
    uint32_t task_id;
    int priority; /* 0 (highest) to 10 (lowest) */
    uint64_t state; /* TASK_READY, TASK_RUNNING, TASK_BLOCKED, TASK_TERMINATED */
    uint64_t deadline; /* For future real-time scheduling */
} task_t;

/* Message priorities */
typedef enum {
    PRIORITY_HIGH = 0, /* Critical commands, interrupts, grid alerts */
    PRIORITY_MEDIUM,   /* Storage operations */
    PRIORITY_LOW       /* Logs, telemetry */
} msg_priority_t;

/* Message structure for prioritized IPC */
typedef struct {
    msg_priority_t priority;
    uint32_t operation; /* OP_SCHEDULE, OP_GRID_ALERT, etc. */
    char payload[MAX_MSG_SIZE];
} message_t;

/* Priority queue for messages */
typedef struct {
    message_t messages[MAX_QUEUE_SIZE];
    int count;
} priority_queue_t;

/* Scheduler interface for hot-swappable schedulers */
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

/* Transaction log entry for updates */
typedef struct {
    uint64_t txn_id;
    char key[64];
    char value[256];
    int operation; /* OP_WRITE, OP_DELETE, etc. */
} txn_log_entry_t;

/* Function prototypes */
void queue_init(priority_queue_t* queue);
int queue_push(priority_queue_t* queue, message_t* msg);
int queue_pop(priority_queue_t* queue, message_t* msg);
int log_message_transaction(message_t* msg);
int log_transaction(txn_log_entry_t* entry);
int commit_transaction(uint64_t txn_id);
int rollback_transaction(uint64_t txn_id);
void scheduler_service(nng_socket socket, scheduler_ops_t* ops);
void log_message(log_level_t level, const char* format, ...);
uint64_t generate_txn_id(void);

#endif /* LUGHOS_H */