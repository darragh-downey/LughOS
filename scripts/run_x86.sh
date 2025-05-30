#!/bin/bash
# Enhanced script for running LughOS in QEMU with GDB debugging

# Ensure the kernel exists
if [ ! -f build/x86/lughos.bin ]; then
    echo "Error: Kernel binary not found at build/x86/lughos.bin"
    echo "Please run 'make x86' first."
    exit 1
fi

# Start QEMU with proper flags for multiboot kernel
qemu-system-i386 \
    -kernel build/x86/lughos.bin \
    -initrd build/x86/user_hello \
    -nographic \
    -no-reboot \
    -no-shutdown \
    -monitor vc \
    -serial stdio \
    -s -S &

QEMU_PID=$!

# Give QEMU a moment to start
sleep 1

# Start GDB and connect to QEMU
gdb build/x86/lughos.bin \
    -ex "target remote localhost:1234" \
    -ex "break kmain" \
    -ex "continue"

# Clean up QEMU process when GDB exits
kill $QEMU_PID 2>/dev/null