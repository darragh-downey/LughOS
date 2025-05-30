#include "lugh.h"

struct idt_entry {
    uint16_t base_lo;
    uint16_t sel;
    uint8_t always0;
    uint8_t flags;
    uint16_t base_hi;
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

// We'll use only entry 0x80 in this simplified version
struct idt_entry idt_entries[256];
struct idt_ptr idt_ptr;

// External handler
extern void syscall_entry(void);

// Internal helper
static void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt_entries[num].base_lo = (uint16_t)(base & 0xFFFF);
    idt_entries[num].base_hi = (uint16_t)((base >> 16) & 0xFFFF);
    idt_entries[num].sel = sel;
    idt_entries[num].always0 = 0;
    idt_entries[num].flags = flags;
}

static void load_idt(struct idt_ptr *ptr) {
    __asm__ volatile("lidt (%0)" : : "r" (ptr));
}

/**
 * Initialize the system call interface
 * 
 * Sets up IDT entry 0x80 for system calls via int $0x80
 */
void init_syscall() {
    log_message(LOG_INFO, "Initializing system call interface\n");

    // Set up IDT for system calls (entry 0x80)
    idt_ptr.limit = sizeof(struct idt_entry) * 256 - 1;
    idt_ptr.base = (uint32_t)&idt_entries;

    // Clear all entries
    memset(&idt_entries, 0, sizeof(struct idt_entry) * 256);

    // Set up system call handler (int 0x80)
    // flags: 0x8E = Present (0x80) | DPL=3 (0x60) | 32-bit interrupt gate (0x0E)
    idt_set_gate(0x80, (uint32_t)syscall_entry, 0x08, 0xEE);

    // Load the IDT
    load_idt(&idt_ptr);

    log_message(LOG_INFO, "System call interface initialized\n");
}
