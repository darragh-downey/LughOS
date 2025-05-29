#include "lugh.h"
#include "security.h"
#include "hardware.h"

extern scheduler_ops_t rr_scheduler;

void kmain(void) {
    // Initialize essential kernel subsystems
    log_message(LOG_INFO, "%s v%s booting...\n", OS_NAME, OS_VERSION);
    
    // Initialize hardware detection and security features
    if (!hw_detect()) {
        log_message(LOG_ERROR, "Hardware detection failed, halting system\n");
        return;
    }
    
    // Initialize security subsystem
    security_init();
    
    // Simple microkernel main loop - for now, just idle
    log_message(LOG_INFO, "Entering kernel main loop\n");
    
    while (1) {
        // Process any pending events
        process_events();
        
        // Idle the CPU to conserve power
        cpu_idle();
    }
}