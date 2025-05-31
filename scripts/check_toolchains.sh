#!/bin/bash

# Script to check toolchain availability
# Returns environment variables that can be used in Makefile

# Check for x86 toolchain
if command -v i686-elf-gcc >/dev/null 2>&1 || [ -d "/opt/i686-elf" ]; then
  echo "X86_TOOLCHAIN_AVAILABLE=1"
else
  echo "X86_TOOLCHAIN_AVAILABLE=0"
fi

# Check for RISC-V toolchain
if command -v riscv64-linux-gnu-gcc >/dev/null 2>&1 || [ -d "/opt/riscv64-linux-gnu" ]; then
  echo "RISCV_TOOLCHAIN_AVAILABLE=1"
else
  echo "RISCV_TOOLCHAIN_AVAILABLE=0"
fi

# Check for ARM toolchain
if command -v arm-none-eabi-gcc >/dev/null 2>&1 || [ -d "/opt/arm-none-eabi" ]; then
  echo "ARM_TOOLCHAIN_AVAILABLE=1"
else
  echo "ARM_TOOLCHAIN_AVAILABLE=0"
fi
