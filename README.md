# LughOS

LughOS is a secure microkernel operating system focused on critical infrastructure, emphasizing security, modularity, and reliability. It follows SEI CERT Secure Coding guidelines and NASA's Power of Ten rules.

## Features

- **Security-focused design**: Implements memory protection, secure I/O, and protection against common vulnerabilities
- **Multiarchitecture support**: Targets both x86 and ARM platforms
- **Modular design**: Microkernel architecture with minimal TCB (Trusted Computing Base)
- **Hardware abstraction**: Clean separation between hardware and higher-level services
- **Built-in security validation**: Runtime and build-time security checks
- **SEI CERT compliant**: Follows SEI CERT C Secure Coding Standard
- **NASA Power of Ten compliant**: Follows NASA's Power of Ten rules for safety-critical code

## Security Features

- **Memory safety**: Strict memory access validation, buffer protection, and pointer validation
- **Static memory allocation**: Fixed-size allocations following NASA Power of Ten rule 3
- **Runtime assertions**: Critical invariant checking per NASA Power of Ten rule 10
- **Data integrity**: Checksum validation of messages and critical data structures
- **Secure messaging**: NNG integration for secure message passing
- **Cryptographic validation**: Message signing and verification for critical operations
- **Explicit padding**: Proper structure padding per SEI CERT DCL39-C
- **Bounds checking**: Fixed loop bounds with explicit checks per NASA Power of Ten rule 2

## Setup (Fedora 42)

### Install dependencies

```bash
sudo dnf install -y gcc-arm-none-eabi binutils-arm-none-eabi qemu-system-x86 qemu-system-arm cmake make git cppcheck
# Install i686-elf-gcc (see instructions below)
```

### Cross-compiler setup

For x86 target development, you need an i686-elf cross-compiler. For instructions, visit: https://wiki.osdev.org/GCC_Cross-Compiler

### Building

Build for x86 platform:

```bash
make x86
```

Build for ARM platform:

```bash
make arm
```

Build both:

```bash
make all
```

### Running

Run the x86 build in QEMU:

```bash
make run
```

Debug with GDB:

```bash
make debug
```

## Security Checks

Run security validation:

```bash
make security-check
```

## Project Structure

- `kernel/`: Core kernel functionality
- `services/`: System services (scheduler, storage, etc.)
- `include/`: Header files
- `lib/`: External libraries
- `scripts/`: Build and runtime scripts
- `docs/`: Documentation
- `tests/`: Test suites

## Current Status

The kernel successfully boots on x86, initializing hardware components, security subsystems, and entering the kernel main loop.
