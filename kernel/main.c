#include "lugh.h"
#include <stdio.h>

static void print(const char *message) {
    printf("%s\n", message); // QEMU console 
}

int kmain(void) {
    char buf[64];
    if (snprintf(buf, sizeof(buf), "%s v%s booting...\n", OS_NAME, OS_VERSION) >= (int)sizeof(buf)) {
        print("Buffer overflow in boot message!");
        return -1; 
    }
    print(buf);
    while(1) {} // halt the kernel
    return 0; 
}