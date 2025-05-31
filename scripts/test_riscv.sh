#!/bin/bash
# RISC-V test script for LughOS

set -e

# Ensure the kernel exists
if [ ! -f build/riscv/lughos.bin ]; then
    echo "Error: RISC-V kernel binary not found at build/riscv/lughos.bin"
    echo "Please run 'make riscv' first."
    exit 1
fi

# Start QEMU with proper flags for RISC-V kernel
qemu-system-riscv64 \
    -machine virt \
    -cpu rv64 \
    -m 128M \
    -kernel build/riscv/lughos.bin \
    -nographic \
    -no-reboot \
    -bios none \
    -monitor null \
    -serial mon:stdio

echo "QEMU RISC-V session ended."
