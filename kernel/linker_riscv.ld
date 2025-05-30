/* RISC-V Linker Script for LughOS */
OUTPUT_FORMAT("elf64-littleriscv", "elf64-littleriscv", "elf64-littleriscv")
OUTPUT_ARCH(riscv)
ENTRY(_start)

SECTIONS
{
	. = 0x80000000;
	
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
		*(COMMON)
		*(.bss)
	}
	
	/* Add a section for stack to make sure it's properly aligned and sized */
	.stack : ALIGN(4K)
	{
		. = . + 0x4000; /* 16KB stack */
		PROVIDE(_stack_top = .);
	}
}
