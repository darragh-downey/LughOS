#ifndef HARDWARE_H
#define HARDWARE_H

#include "lugh.h"

/**
 * Hardware abstraction layer for LughOS
 * 
 * This module provides hardware-specific functions for
 * different architectures supported by LughOS.
 */

// Hardware detection and initialization
int hw_detect(void);
void hw_init(void);

// CPU operations
void cpu_halt(void); // Halt CPU completely

// Hardware-specific memory management operations
void hw_enable_memory_protection(void);
void hw_map_page(void* virt_addr, void* phys_addr, uint32_t flags);
void hw_unmap_page(void* virt_addr);

// Hardware-specific shutdown procedures
void hw_shutdown(void);
void hw_reboot(void);

// Power management
void hw_power_save(uint8_t level);

// Timer operations
void hw_init_timer(uint32_t frequency);
uint64_t hw_get_ticks(void);

// Note: The following functions are already declared in lugh.h,
// so we've removed them from this header to avoid redundant declarations:
// - outb
// - inb
// - cpu_idle
// - process_events

#endif // HARDWARE_H