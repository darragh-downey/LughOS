#!/bin/bash
# Enhanced RISC-V test script for LughOS with OpenSBI

set -e

# Check if OpenSBI firmware exists
OPENSBI="/usr/share/qemu/opensbi-riscv64-generic-fw_dynamic.bin"
if [ ! -f "$OPENSBI" ]; then
    echo "OpenSBI firmware not found at $OPENSBI"
    echo "OpenSBI firmware might be in a different location."
    exit 1
fi

# Ensure the kernel exists
if [ ! -f build/riscv/lughos.bin ]; then
    echo "Error: RISC-V kernel binary not found at build/riscv/lughos.bin"
    echo "Please run 'make riscv' first."
    exit 1
fi

# Start QEMU with OpenSBI firmware and LughOS as payload
qemu-system-riscv64 \
    -machine virt \
    -cpu rv64 \
    -m 128M \
    -bios $OPENSBI \
    -kernel build/riscv/lughos.bin \
    -nographic \
    -no-reboot \
    -monitor null \
    -serial mon:stdio \
    -append "console=ttyS0"

echo "QEMU RISC-V session ended."
