# LughOS CI/CD Pipeline

This document describes the Continuous Integration and Continuous Deployment (CI/CD) pipeline for LughOS.

## Overview

The CI/CD pipeline for LughOS consists of three main workflows:

1. **Build**: Builds the kernel and tests for multiple architectures (x86, ARM, RISC-V)
2. **Matrix Build**: Builds the kernel in parallel for all architectures
3. **Static Analysis**: Performs code quality and security checks

## Build Workflow

The build workflow (`build.yml`) does the following:

- Installs dependencies
- Sets up cross-compilation toolchains (i686-elf-gcc, arm-none-eabi-gcc, riscv64-linux-gnu-gcc)
- Builds the kernel for x86, ARM, and RISC-V architectures
- Runs tests using QEMU
- Uploads build artifacts and test logs

### Caching

The build workflow uses GitHub Actions caching for:
- i686-elf toolchain
- NNG library build

### Environment Variables

Each build step has explicit PATH environment variables to ensure the toolchains are found.

## Matrix Build Workflow

The matrix build workflow (`matrix_build.yml`) runs builds for different architectures in parallel:

- Sets up a shared environment in a preliminary job
- Uses a build matrix to run architecture-specific builds
- Collects all artifacts into a combined test report

## Static Analysis Workflow

The static analysis workflow (`analysis.yml`) performs:

- CERT C compliance checks with clang-tidy
- General code quality checks with Cppcheck
- Security vulnerability scanning
- Generates HTML reports for analysis results

## Running Tests Locally

To run tests locally, use the following commands:

```bash
# Run all tests
make test

# Run specific architecture tests
./scripts/run_unit_tests.sh x86
./scripts/run_unit_tests.sh arm
./scripts/run_unit_tests.sh riscv

# Run CI pipeline locally
make ci
```

## Test Reports

Test reports are generated in the `test-logs` directory. For CI runs, these are uploaded as artifacts.

## Static Analysis

Static analysis reports are generated in the `reports` directory. For CI runs, these are uploaded as artifacts.

## Security Scanning

LughOS uses CERT C static analysis via clang-tidy to detect security issues. The workflow is configured to check:
- Memory safety issues
- Undefined behavior
- Buffer overflows
- Integer handling issues
- Proper error checking

## Debugging Failed CI Runs

When a CI run fails:

1. Check the "Actions" tab in GitHub
2. Download the artifacts for logs and test reports
3. Look for specific failures in the test logs
4. If the issue is with the compiler not being found, check the PATH settings and outputs of the "Verify compiler existence" step
