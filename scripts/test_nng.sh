#!/bin/bash

# Test script for LughOS NNG implementation
# This script builds and runs LughOS with our NNG implementation

# Set up colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${BLUE}=== LughOS NNG Test Script ===${NC}"

# Clean and build
echo -e "${YELLOW}Cleaning build directory...${NC}"
make clean

echo -e "${YELLOW}Building LughOS...${NC}"
make x86 || { echo -e "${RED}Build failed!${NC}"; exit 1; }

echo -e "${GREEN}Build successful!${NC}"

# Run in QEMU
echo -e "${YELLOW}Running LughOS in QEMU...${NC}"
echo -e "${BLUE}Look for NNG test messages in the output${NC}"

# Run with QEMU
./scripts/test_x86.sh

echo -e "${GREEN}Test completed. Verify output for NNG test messages.${NC}"
