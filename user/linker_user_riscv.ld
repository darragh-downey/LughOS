/* RISC-V User Mode Program Linker Script for LughOS */
ENTRY(_start)

SECTIONS
{
    /* Code begins at a reasonable offset in virtual memory */
    . = 0x400000;

    /* Code section */
    .text :
    {
        *(.text.entry)
        *(.text*)
    }

    /* Read-only data (constants) */
    .rodata :
    {
        *(.rodata*)
    }

    /* Read-write data */
    .data :
    {
        *(.data*)
    }

    /* Uninitialized data */
    .bss :
    {
        *(COMMON)
        *(.bss*)
    }

    /* Stack starts at a safe distance from the heap */
    . = 0x700000;
    _stack_top = .;
}
