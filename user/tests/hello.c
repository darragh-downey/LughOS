/**
 * Hello World test program for LughOS user mode
 *
 * This is a simple test program that demonstrates basic user mode
 * functionality in LughOS.
 */
#include "../lib/user.h"

int main(void) {
    print("Hello from LughOS user mode!\n");
    print("Testing system calls and basic functionality...\n\n");
    
    // Test IPC
    print("Sending regular IPC message...\n");
    send_ipc(OP_WRITE, "Test IPC message from user mode");
    
    // Test critical infrastructure alert
    print("Sending grid alert message...\n");
    send_grid_alert("GRID_ALERT: Test alert from user mode");
    
    print("\nAll tests complete. Exiting with status 0.\n");
    return 0;
}
