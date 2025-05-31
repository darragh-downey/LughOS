#!/bin/bash

# Script to fix toolchain paths for cross-compilers

# Check for x86 toolchain and set paths if found
if command -v i686-elf-gcc >/dev/null 2>&1 || [ -d "/opt/i686-elf" ]; then
  export PATH="/opt/i686-elf/bin:/opt/i686-elf/i686-elf/bin:$PATH"
  export LIBRARY_PATH="/opt/i686-elf/lib:/opt/i686-elf/i686-elf/lib"
  export C_INCLUDE_PATH="/opt/i686-elf/include:/opt/i686-elf/i686-elf/include"
  export CPLUS_INCLUDE_PATH="/opt/i686-elf/include:/opt/i686-elf/i686-elf/include"
  export X86_TOOLCHAIN_AVAILABLE=1
  echo "X86 toolchain detected"
else
  export X86_TOOLCHAIN_AVAILABLE=0
  echo "Warning: X86 toolchain not available"
fi

# Check for RISC-V toolchain
if command -v riscv64-linux-gnu-gcc >/dev/null 2>&1 || [ -d "/opt/riscv64-linux-gnu" ]; then
  export PATH="/opt/riscv64-linux-gnu/bin:$PATH"
  export LIBRARY_PATH="/opt/riscv64-linux-gnu/lib:$LIBRARY_PATH"
  export C_INCLUDE_PATH="/opt/riscv64-linux-gnu/include:$C_INCLUDE_PATH"
  export CPLUS_INCLUDE_PATH="/opt/riscv64-linux-gnu/include:$CPLUS_INCLUDE_PATH"
  export RISCV_TOOLCHAIN_AVAILABLE=1
  echo "RISC-V toolchain detected"
else
  export RISCV_TOOLCHAIN_AVAILABLE=0
  echo "Warning: RISC-V toolchain not available"
fi

# Check for ARM toolchain
if command -v arm-none-eabi-gcc >/dev/null 2>&1 || [ -d "/opt/arm-none-eabi" ]; then
  export PATH="/opt/arm-none-eabi/bin:$PATH"
  export LIBRARY_PATH="/opt/arm-none-eabi/lib:$LIBRARY_PATH"
  export C_INCLUDE_PATH="/opt/arm-none-eabi/include:$C_INCLUDE_PATH"
  export CPLUS_INCLUDE_PATH="/opt/arm-none-eabi/include:$CPLUS_INCLUDE_PATH"
  export ARM_TOOLCHAIN_AVAILABLE=1
  echo "ARM toolchain detected"
else
  export ARM_TOOLCHAIN_AVAILABLE=0
  echo "Warning: ARM toolchain not available"
fi

echo "Toolchain paths have been set up:"
echo "PATH=$PATH"
echo "LIBRARY_PATH=$LIBRARY_PATH"
echo "C_INCLUDE_PATH=$C_INCLUDE_PATH"

# Check if the command is a toolchain command and if the appropriate toolchain is available
cmd=$(basename "$1")

if [[ "$cmd" == "i686-elf-"* && "$X86_TOOLCHAIN_AVAILABLE" -ne 1 ]]; then
  echo "Error: Attempting to use x86 toolchain ($cmd) but it's not available"
  exit 1
fi

if [[ "$cmd" == "riscv64-linux-gnu-"* && "$RISCV_TOOLCHAIN_AVAILABLE" -ne 1 ]]; then
  echo "Error: Attempting to use RISC-V toolchain ($cmd) but it's not available"
  exit 1
fi

if [[ "$cmd" == "arm-none-eabi-"* && "$ARM_TOOLCHAIN_AVAILABLE" -ne 1 ]]; then
  echo "Error: Attempting to use ARM toolchain ($cmd) but it's not available"
  exit 1
fi

# Execute the command that was passed in
exec "$@"
