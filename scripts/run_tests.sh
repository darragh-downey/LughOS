#!/bin/bash
# Enhanced test runner for LughOS CI/CD
# This script runs various tests and collects the results

set -e

# Create test output directory
mkdir -p test-logs

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Function to run a test and log the result
run_test() {
    local test_name="$1"
    local test_cmd="$2"
    local test_log="test-logs/${test_name}.log"
    
    echo -e "${YELLOW}Running test: ${test_name}${NC}"
    echo "Command: ${test_cmd}"
    echo "==================== Test: ${test_name} ====================" > "${test_log}"
    echo "Command: ${test_cmd}" >> "${test_log}"
    echo "Output:" >> "${test_log}"
    
    # Run the test and capture output
    if bash -c "${test_cmd}" >> "${test_log}" 2>&1; then
        echo -e "${GREEN}✓ Test ${test_name} passed${NC}"
        echo "RESULT: PASSED" >> "${test_log}"
        return 0
    else
        echo -e "${RED}✗ Test ${test_name} failed${NC}"
        echo "RESULT: FAILED" >> "${test_log}"
        # Show the last few lines of the log for quick debugging
        echo "Last lines of log:"
        tail -n 10 "${test_log}"
        return 1
    fi
}

# Make sure we have the binaries
if [ ! -f build/x86/lughos.bin ]; then
    echo -e "${RED}Error: x86 kernel binary not found. Run 'make x86' first.${NC}"
    exit 1
fi

# Create a markdown test report
echo "# LughOS Test Results" > test-logs/test-report.md
echo "Generated on: $(date)" >> test-logs/test-report.md
echo "" >> test-logs/test-report.md

# Run our tests
test_results=()

# Test 1: Basic QEMU boot test for x86
if run_test "qemu_boot_x86" "timeout 30s qemu-system-i386 -kernel build/x86/lughos.bin -initrd build/x86/user_hello -nographic -no-reboot -serial stdio -display none"; then
    test_results+=("✓ qemu_boot_x86")
else
    test_results+=("✗ qemu_boot_x86")
fi

# Add test results to the report
echo "## Test Results" >> test-logs/test-report.md
echo "" >> test-logs/test-report.md
for result in "${test_results[@]}"; do
    echo "- ${result}" >> test-logs/test-report.md
done

echo "" >> test-logs/test-report.md
echo "See log files for details." >> test-logs/test-report.md

echo -e "${YELLOW}Tests completed. See test-logs/ directory for details.${NC}"
exit 0
