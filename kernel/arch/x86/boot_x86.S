// Multiboot header for GRUB/QEMU compatibility
.set ALIGN,    1<<0             // align loaded modules on page boundaries
.set MEMINFO,  1<<1             // provide memory map
.set FLAGS,    ALIGN | MEMINFO  // this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002       // 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) // checksum of above, to prove we are multiboot

// Multiboot header section
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

// Allocate initial stack
.section .bootstrap_stack, "aw", @nobits
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

// Entry point
.section .text
.global _start
.type _start, @function

_start:
    // Set up the stack
    mov $stack_top, %esp
    
    // Clear interrupts
    cli
    
    // Reset EFLAGS
    pushl $0
    popf
    
    // Call kernel main function
    call kmain
    
    // Hang if kernel returns
    cli
hang:
    hlt
    jmp hang
    
.size _start, . - _start