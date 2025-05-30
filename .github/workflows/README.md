# LughOS GitHub Actions Workflows

This directory contains GitHub Actions workflows for building, testing, and analyzing LughOS.

## Available Workflows

### 1. Build Workflow (`build.yml`)

The main workflow for building LughOS and running basic tests.

- Builds for x86, ARM, and RISC-V architectures
- Installs the required toolchains and dependencies
- Runs QEMU tests
- Uploads build artifacts and test logs

### 2. Matrix Build Workflow (`matrix_build.yml`)

A more efficient version of the build workflow that uses a matrix strategy to build multiple architectures in parallel.

- Builds for x86, ARM, and RISC-V architectures in parallel
- Shares toolchain artifacts between jobs
- Generates a combined test report

### 3. Static Analysis Workflow (`analysis.yml`)

Performs code quality and security checks.

- Runs clang-tidy with CERT C compliance checks
- Runs Cppcheck for general code quality
- Generates HTML reports for analysis results

### 4. Reusable Workflow (`reusable.yml`)

A reusable workflow that can be imported by other projects that use LughOS.

#### Usage Example

```yaml
name: My LughOS-based Project CI
on:
  push:
    branches: [ main ]
  pull_request:
    branches: [ main ]

jobs:
  lughos-ci:
    uses: YourOrg/LughOS/.github/workflows/reusable.yml@main
    with:
      architectures: '["x86", "arm"]'
      run-tests: true
      run-analysis: true
    permissions:
      contents: read
      packages: read
      id-token: write
      actions: read
```

## Common Issues and Solutions

### Missing Toolchain

If you encounter errors with the i686-elf-gcc toolchain not being found:

1. Check the PATH setup in the workflow
2. Verify that the toolchain files have the correct execute permissions
3. Look at the "Verify compiler existence" step output 

### Failed Tests

Test failures will be available in the test logs artifact. Download this artifact to see detailed error messages.

### New Architectures

To add a new architecture:

1. Add it to the matrix in `matrix_build.yml`
2. Create appropriate build targets in the Makefile
3. Add architecture-specific test scripts in the scripts directory
