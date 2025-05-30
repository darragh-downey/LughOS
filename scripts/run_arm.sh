#!/bin/bash
# Enhanced script for running LughOS ARM in QEMU with GDB debugging

# Ensure the kernel exists
if [ ! -f build/arm/lughos.bin ]; then
    echo "Error: ARM kernel binary not found at build/arm/lughos.bin"
    echo "Please run 'make arm' first."
    exit 1
fi

# Start QEMU with proper flags
qemu-system-arm \
    -M versatilepb \
    -kernel build/arm/lughos.bin \
    -initrd build/arm/user_hello \
    -nographic \
    -serial mon:stdio \
    -s -S &

QEMU_PID=$!

# Give QEMU a moment to start
sleep 1

# Start GDB and connect to QEMU
arm-none-eabi-gdb build/arm/lughos.bin \
    -ex "target remote localhost:1234" \
    -ex "break kmain" \
    -ex "continue"

# Clean up QEMU process when GDB exits
kill $QEMU_PID 2>/dev/null
