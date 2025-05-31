/* RISC-V Linker Script for LughOS */
OUTPUT_FORMAT("elf64-littleriscv", "elf64-littleriscv", "elf64-littleriscv")
OUTPUT_ARCH(riscv)
ENTRY(_start)

SECTIONS
{
	/* Start at 0x80100000 to avoid conflicting with OpenSBI firmware */
	. = 0x80100000;
	
	.text : ALIGN(4K)
	{
		*(.text.boot)
		*(.text)
	}
	
	.rodata : ALIGN(4K)
	{
		*(.rodata)
	}
	
	.data : ALIGN(4K)
	{
		*(.data)
	}
	
	.bss : ALIGN(4K)
	{
		PROVIDE(__bss_start = .);
		*(COMMON)
		*(.bss)
		PROVIDE(__bss_end = .);
	}
	
	/* Add a section for stack to make sure it's properly aligned and sized */
	.stack : ALIGN(4K)
	{
		. = . + 0x4000; /* 16KB stack */
		PROVIDE(_stack_top = .);
	}
}
