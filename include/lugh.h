#ifndef LUGHOS_H
#define LUGHOS_H

/* Use standard headers for types */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Define NULL if not already defined */
#ifndef NULL
#define NULL ((void*)0)
#endif

/* Variable arguments (for printf-like functions) */
typedef __builtin_va_list va_list;
#define va_start(v,l) __builtin_va_start(v,l)
#define va_end(v)     __builtin_va_end(v)
#define va_arg(v,l)   __builtin_va_arg(v,l)

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
#define OP_UPDATE 0x102     /* System update operation */
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
    int priority;      /* 0 (highest) to 10 (lowest) */
    uint32_t _padding1;/* Explicit padding per CERT DCL39-C */
    uint64_t state;    /* TASK_READY, TASK_RUNNING, TASK_BLOCKED, TASK_TERMINATED */
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
    uint32_t operation;             /* OP_SCHEDULE, OP_GRID_ALERT, etc. */
    uint32_t checksum;              /* Integrity check per NASA Power of Ten rule 6 */
    uint32_t _padding1;             /* Explicit padding per CERT DCL39-C */
    char payload[MAX_MSG_SIZE];     /* Always terminated with '\0' per CERT STR31-C */
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
    char key[64];             /* Always terminated with '\0' per CERT STR31-C */
    char value[256];          /* Always terminated with '\0' per CERT STR31-C */
    int operation;            /* OP_WRITE, OP_DELETE, etc. */
    uint32_t checksum;        /* Integrity check per NASA Power of Ten rule 6 */
    uint8_t _padding[4];      /* Explicit padding per CERT DCL39-C to ensure 8-byte alignment */
} txn_log_entry_t;

/* Memory safety functions */
void* memcpy(void* dest, const void* src, size_t n);
void* memmove(void* dest, const void* src, size_t n);
void* memset(void* s, int c, size_t n);
size_t strlen(const char* s);
int strcmp(const char* s1, const char* s2);
char* strcpy(char* dest, const char* src);
char* strstr(const char* haystack, const char* needle);
void* kmalloc(size_t size);
void kfree(void* ptr);

/* IO port functions */
void outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);

/* Function prototypes */
void queue_init(priority_queue_t* queue);
int queue_push(priority_queue_t* queue, message_t* msg);
int queue_pop(priority_queue_t* queue, message_t* msg);

/* Data integrity functions per NASA Power of Ten rule 6 */
uint32_t calculate_checksum(const void* data, size_t len);
// moved to transactions.h
// int log_message_transaction(message_t* msg);
// int log_transaction(txn_log_entry_t* entry);
// int commit_transaction(uint64_t txn_id);
// int rollback_transaction(uint64_t txn_id);
void scheduler_service(void* socket, scheduler_ops_t* ops);
void log_message(log_level_t level, const char* format, ...);
uint64_t generate_txn_id(void);
uint64_t generate_secure_id(void);
void process_events(void);
void cpu_idle(void);
void enter_user_mode(uint32_t user_eip, uint32_t user_esp) __attribute__((noreturn));

/* User mode functions */
void switch_to_user_mode(uint32_t user_eip, uint32_t user_esp);

// moved to memory.h
// int load_user_program(const void* binary_ptr, size_t size, uint32_t* eip, uint32_t* esp);

/* Initialize system call mechanism */
void init_syscall(void);
void init_syscall_arm(void);
void init_syscall_riscv_c(void);

/* System call definitions */
#define SYS_WRITE    1    /* Write to console */
#define SYS_IPC_SEND 2    /* Send IPC message */
#define SYS_EXIT     3    /* Exit program */

#ifdef __KERNEL__
void syscall_handler(uint32_t num, uint32_t arg1, uint32_t arg2, uint32_t arg3);
#endif

#endif /* LUGHOS_H */