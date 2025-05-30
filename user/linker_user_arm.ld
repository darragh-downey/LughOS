/* User-space linker script for ARM LughOS applications
 * This places user programs at a fixed address (0x400000) with proper memory layout
 */
OUTPUT_FORMAT("elf32-littlearm")
ENTRY(_start)

PHDRS
{
    text PT_LOAD FLAGS(5);    /* R-X */
    rodata PT_LOAD FLAGS(4);  /* R-- */
    data PT_LOAD FLAGS(6);    /* RW- */
    bss PT_LOAD FLAGS(6);     /* RW- */
}

SECTIONS {
    /* Start at 0x400000 for ARM user programs */
    . = 0x400000;

    .text : {
        *(.text)
        *(.text.*)
    } :text

    .rodata : {
        *(.rodata)
        *(.rodata.*)
    } :rodata

    .data : {
        *(.data)
        *(.data.*)
    } :data

    .bss : {
        *(COMMON)
        *(.bss)
        *(.bss.*)
    } :bss
}
