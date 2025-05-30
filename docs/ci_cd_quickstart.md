# LughOS CI/CD Quick Start Guide

This guide provides quick instructions for setting up and using the LughOS CI/CD pipeline.

## Setting Up CI/CD for Your Project

### 1. Using the Reusable Workflow

If you have a LughOS-based project, you can use our reusable workflow:

1. Create a `.github/workflows` directory in your project
2. Create a workflow file (e.g., `ci.yml`) with the following content:

```yaml
name: CI
on:
  push:
    branches: [ main ]
  pull_request:
    branches: [ main ]

jobs:
  lughos-ci:
    uses: YourOrg/LughOS/.github/workflows/reusable.yml@main
    with:
      architectures: '["x86", "arm", "riscv"]'
      run-tests: true
      run-analysis: true
    permissions:
      contents: read
      packages: read
      id-token: write
      actions: read
```

### 2. Setting Up Your Own Workflow

If you need a custom workflow, you can copy and adapt our workflows:

1. Copy the relevant workflow file(s) from LughOS
2. Modify as needed for your project
3. Ensure proper permissions are set

## Running Locally

### Building

```bash
# Build for all architectures
make all

# Build for specific architecture
make x86
make arm
make riscv
```

### Testing

```bash
# Run all tests
make test

# Run tests for specific architecture
./scripts/run_unit_tests.sh x86
```

### Static Analysis

```bash
# Run all analysis tools
make analyze

# Run security checks
make security-check

# Run complete CI pipeline locally
make ci
```

## Troubleshooting

### Common Issues

1. **Missing toolchain**: Ensure i686-elf-gcc is installed and in your PATH
2. **Build failures**: Check individual architecture logs in the build directory
3. **Test failures**: Look at test logs in the test-logs directory

### CI Failures

1. Check GitHub Actions logs for specific errors
2. Download artifacts for detailed logs
3. Look for toolchain or compiler path issues
4. Verify that all required permissions are set correctly

## Adding a New Architecture

1. Add build targets in the Makefile
2. Create linker script in the kernel directory
3. Update matrix build workflow to include the new architecture
4. Add test scripts for the new architecture
