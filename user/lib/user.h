#ifndef USER_H
#define USER_H

#include <stdint.h>
#include <stddef.h>

/* System call numbers */
#define SYS_WRITE    1
#define SYS_IPC_SEND 2
#define SYS_EXIT     3

/* Message operations - same as kernel definitions */
#define OP_ADD_TASK 0x01
#define OP_SCHEDULE 0x02
#define OP_GRID_ALERT 0x100 /* Critical infrastructure: energy grid fault */
#define OP_HEARTBEAT 0x101  /* Future: distributed operation */
#define OP_WRITE 0x200
#define OP_DELETE 0x201

/* Message priorities */
typedef enum {
    PRIORITY_HIGH = 0,
    PRIORITY_MEDIUM,
    PRIORITY_LOW
} msg_priority_t;

#define MAX_MSG_SIZE 128

/* Message structure for IPC */
typedef struct {
    msg_priority_t priority;
    uint32_t operation;
    uint32_t checksum;
    uint32_t _padding1;
    char payload[MAX_MSG_SIZE];
} message_t;

/* System call function */
void syscall(uint32_t num, uint32_t arg1, uint32_t arg2, uint32_t arg3);

/* User library functions */
void write(const char* buf, size_t len);
void print(const char* str);
size_t strlen(const char* str);
void* memcpy(void* dest, const void* src, size_t n);
void exit(int code);
int send_ipc(uint32_t operation, const char* message);
int send_grid_alert(const char* message);

#endif /* USER_H */
