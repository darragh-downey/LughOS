/* filepath: /home/ddowney/Workspace/github.com/LughOS/kernel/linker_x86.ld */
OUTPUT_FORMAT("elf32-i386")
ENTRY(_start)

SECTIONS {
    /* Begin at 1MB - standard location for kernels to be loaded by GRUB */
    . = 1M;

    /* First put the multiboot header, as it needs to be within the first 8KB */
    .text BLOCK(4K) : ALIGN(4K) {
        *(.multiboot)
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

    /* The compiler may produce other sections, put them in the proper place in
       in this file, if needed */
}