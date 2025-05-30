#!/bin/bash
# LughOS Unit Test Runner
# This script runs unit tests for specific architectures

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Default architecture is x86
ARCH=${1:-x86}

echo -e "${YELLOW}Running unit tests for architecture: ${ARCH}${NC}"

# Create logs directory
mkdir -p test-logs

# Function to run a single unit test
run_unit_test() {
    local test_name="$1"
    local test_bin="$2"
    local test_log="test-logs/unit_${ARCH}_${test_name}.log"
    
    echo -e "${YELLOW}Running unit test: ${test_name}${NC}"
    echo "==================== Unit Test: ${test_name} ====================" > "${test_log}"
    
    if [ -f "${test_bin}" ]; then
        # Execute the test (could be directly or via QEMU based on architecture)
        case "${ARCH}" in
            x86)
                qemu-system-i386 -kernel "${test_bin}" -nographic -no-reboot -serial stdio -display none -append "test" >> "${test_log}" 2>&1 || { 
                    echo -e "${RED}✗ Test ${test_name} failed${NC}"
                    echo "RESULT: FAILED" >> "${test_log}"
                    return 1
                }
                ;;
            arm)
                qemu-system-arm -kernel "${test_bin}" -nographic -no-reboot -serial stdio -display none -machine virt >> "${test_log}" 2>&1 || {
                    echo -e "${RED}✗ Test ${test_name} failed${NC}"
                    echo "RESULT: FAILED" >> "${test_log}"
                    return 1
                }
                ;;
            *)
                echo -e "${RED}Unknown architecture: ${ARCH}${NC}"
                return 1
                ;;
        esac
        
        echo -e "${GREEN}✓ Test ${test_name} passed${NC}"
        echo "RESULT: PASSED" >> "${test_log}"
        return 0
    else
        echo -e "${RED}Test binary not found: ${test_bin}${NC}"
        echo "Test binary not found: ${test_bin}" >> "${test_log}"
        echo "RESULT: SKIPPED" >> "${test_log}"
        return 1
    fi
}

# Check if we have any tests to run
TEST_DIR="build/${ARCH}/tests"
if [ ! -d "${TEST_DIR}" ]; then
    echo -e "${RED}No tests found for architecture ${ARCH}${NC}"
    exit 1
fi

# Create a markdown test report
echo "# LughOS Unit Test Results for ${ARCH}" > "test-logs/unit_test_report_${ARCH}.md"
echo "Generated on: $(date)" >> "test-logs/unit_test_report_${ARCH}.md"
echo "" >> "test-logs/unit_test_report_${ARCH}.md"
echo "## Test Results" >> "test-logs/unit_test_report_${ARCH}.md"

# Run all tests in the test directory
test_results=()
FAILED=0

# For now, we'll just run the kernel and user_hello as basic smoke tests
# In a real scenario, you'd iterate through dedicated test binaries
if [ -f "build/${ARCH}/lughos.bin" ]; then
    if run_unit_test "kernel_boot" "build/${ARCH}/lughos.bin"; then
        test_results+=("✓ kernel_boot")
    else
        test_results+=("✗ kernel_boot")
        FAILED=1
    fi
fi

if [ -f "build/${ARCH}/user_hello" ]; then
    if run_unit_test "user_hello" "build/${ARCH}/user_hello"; then
        test_results+=("✓ user_hello")
    else
        test_results+=("✗ user_hello")
        FAILED=1
    fi
fi

# Add test results to the report
for result in "${test_results[@]}"; do
    echo "- ${result}" >> "test-logs/unit_test_report_${ARCH}.md"
done

if [ ${FAILED} -eq 1 ]; then
    echo -e "${RED}Some tests failed!${NC}"
    exit 1
else
    echo -e "${GREEN}All tests passed!${NC}"
    exit 0
fi
