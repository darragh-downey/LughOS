/* User-space linker script for x86 LughOS applications
 * This places user programs at a fixed address (0x400000) with proper memory layout
 */
OUTPUT_FORMAT("elf32-i386")
ENTRY(_start)

SECTIONS {
    /* Begin at 4MB - common user space starting address */
    . = 0x400000;

    /* First put the multiboot header, as it needs to be within the first 8KB */
    .text BLOCK(4K) : ALIGN(4K) {
        *(.text)
    }

    /* Read-only data */
    .rodata BLOCK(4K) : ALIGN(4K) {
        *(.rodata)
    }

    /* Read-write data (initialized) */
    .data BLOCK(4K) : ALIGN(4K) {
        *(.data)
    }

    /* Read-write data (uninitialized) and stack */
    .bss BLOCK(4K) : ALIGN(4K) {
        *(COMMON)
        *(.bss)
        *(.bootstrap_stack)
    }
}
