OUTPUT_FORMAT("elf32-littlearm")
ENTRY(_start)
SECTIONS {
    . = 0x10000;
    .text : { *(.text) }
    .data : { *(.data) }
    .bss : { *(.bss) }
}