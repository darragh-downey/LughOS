# LughOS

LughOS is a secure microkernel operating system focused on critical infrastructure, emphasizing security, modularity, and reliability.

## Features

- **Security-focused design**: Implements memory protection, secure I/O, and protection against common vulnerabilities
- **Multiarchitecture support**: Targets both x86 and ARM platforms
- **Modular design**: Microkernel architecture with minimal TCB (Trusted Computing Base)
- **Hardware abstraction**: Clean separation between hardware and higher-level services
- **Built-in security validation**: Runtime and build-time security checks

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
