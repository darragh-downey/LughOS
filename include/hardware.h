#ifndef HARDWARE_H
#define HARDWARE_H

#include "lugh.h"

/**
 * Detect available hardware and initialize security features
 * 
 * @return true on successful hardware initialization
 */
bool hw_detect(void);

/**
 * Helper function to output a byte to an I/O port on x86
 */
void outb(uint16_t port, uint8_t val);

/**
 * Helper function to input a byte from an I/O port on x86
 */
uint8_t inb(uint16_t port);

/**
 * Put the CPU into idle state (power saving)
 */
void cpu_idle(void);

/**
 * Process any pending hardware events
 */
void process_events(void);

#endif /* HARDWARE_H */
