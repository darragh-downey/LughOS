# LughOS TODO List

Comprehensive TODO list for LughOS, a highly composable, reliable, and secure microkernel OS for critical infrastructure (energy sector focus), featuring hot-swappable schedulers, prioritized messaging, transactional updates, and zero downtime. Targets x86/ARM on Fedora 42 with QEMU, as of May 29, 2025.

## Build System

### High Priority

- [x] **Fix Multiboot Header for x86 Boot**
  - Updated boot_x86.S with proper multiboot header
  - Modified linker script to place multiboot header at start of binary
  - Verified working with QEMU
  
- [ ] **Fix NNG Integration**
  - Verify lib/nng/include/nng/nng.h and lib/nng/lib/libnng.a
  - Update Makefile to properly link with NNG for bare metal
  - Test with cross-compilers for both architectures
  - Debug with: `i686-elf-gcc -v -E - </dev/null 2>&1 | grep include`
  - Notes: Critical for implementing message passing

- [ ] **Serial Console Improvements**
  - Add configurable serial port options
  - Add proper scrolling and buffer management
  - Implement VT100 terminal escape sequences for better formatting
  - Notes: Enhances debugging capabilities

### Medium Priority

- [ ] **Optimize Makefile**
  - Add dependency tracking (.d files)
  - Support make -j$(nproc) for parallel builds
  - Add component tests (e.g., make test-ipc)
  - Notes: Speeds up development

- [ ] **Cross-Compiler Enhancements**
  - Document exact toolchain requirements
  - Create scripts to verify toolchain compatibility
  - Add support for additional architectures (RISC-V)
  - Notes: Ensures portability

## Kernel Development

### High Priority

- [x] **Basic x86 Boot Process**
  - Implemented multiboot-compatible boot code
  - Added proper stack setup and initialization
  - Added QEMU debugging support

- [ ] **Implement Secure IPC**
  - Use message validation with cryptographic checksums:

    ```c
    if (!validate_message(&msg)) {
        log_message(LOG_ERROR, "Invalid message\n");
        return -1;
    }
    ```

  - Implement priority-based message queueing for energy sector operations
  - Test critical operations like OP_GRID_ALERT
  - Notes: Ensures secure, reliable messaging

- [ ] **Memory Protection**
  - Configure x86 MMU for proper kernel/user separation
  - Implement page allocation with security boundaries
  - Add per-component security domains
  - Notes: Critical for microkernel isolation

- [ ] **Enhance Scheduler Service**
  - Complete round_robin.c implementation
  - Implement scheduler_ops_t interface for hot-swapping
  - Test swapping schedulers during runtime
  - Notes: Supports composability, zero downtime

### Medium Priority

- [ ] **Task Management** (3 days, by June 10, 2025)
  - Complete task.c implementation:

    ```c
    int create_task(task_t* task) {
        task->task_id = generate_secure_id();
        task->state = TASK_READY;
        task->security_domain = current_domain;
        return 0;
    }
    ```

  - Implement task isolation with security boundaries
  - Add resource accounting
  - Notes: Enables reliable task execution

- [ ] **Memory Safety Enhancements**
  - Update memory.c with bounds checking:

    ```c
    void* alloc_memory(size_t size) {
        if (size > MAX_ALLOC_SIZE || size == 0) {
            log_message(LOG_ERROR, "Invalid allocation size: %zu\n", size);
            return NULL;
        }
        // Secure allocation logic
    }
    ```

  - Use KERNEL_PAGE_SIZE for proper page alignment
  - Add memory sanitization on free
  - Notes: Critical for security

### Low Priority

- [ ] **Real-Time Scheduling**
  - Implement deadline-based scheduling for energy grid tasks
  - Add jitter analysis and mitigation
  - Notes: Important for deterministic behavior

## Services

### High Priority

- [ ] **Transactional Storage Service**
  - Implement storage_ops_t interface with transaction support:

    ```c
    int log_transaction(txn_log_entry_t* entry) {
        log_message(LOG_INFO, "Transaction %lu: %s=%s\n", 
                   entry->txn_id, entry->key, entry->value);
        return validate_and_store(entry);
    }
    ```

  - Add commit/rollback functionality
  - Test with scheduler hot-swapping
  - Notes: Ensures reliable updates

- [ ] **Hardware Abstraction Enhancements**
  - Complete hardware.c with platform detection
  - Add power management features for energy efficiency
  - Implement secure I/O operations
  - Notes: Increases portability and security

### Medium Priority

- [ ] **Priority Scheduler**
  - Implement priority.c with energy sector specific prioritization
  - Add priority inheritance to prevent priority inversion
  - Test hot-swapping between schedulers
  - Notes: Critical for responsiveness

- [ ] **Security Monitoring Service**
  - Implement intrusion detection for kernel operations
  - Add secure logging of security events
  - Create security policy enforcement points
  - Notes: Enhances security posture

## Testing

### High Priority

- [x] **Basic QEMU Testing**
  - Added scripts/test_x86.sh for quick testing
  - Enhanced scripts/run_x86.sh for GDB debugging
  - Added serial console output

- [ ] **Comprehensive QEMU Testing**
  - Test ARM support with scripts/run_arm.sh
  - Verify startup messages and kernel initialization
  - Test IPC with priority messages
  - Notes: Validates core functionality

- [ ] **Security and Reliability Tests**
  - Test message validation
  - Simulate corrupted messages and buffer overflows
  - Test scheduler hot-swapping with transactions
  - Notes: Ensures secure, reliable operation

### Medium Priority

- [ ] **Energy Grid Simulation**
  - Implement grid operation simulator
  - Test high-frequency alert handling:

    ```c
    for (int i = 0; i < 1000; i++) {
        message_t alert = {
            .priority = PRIORITY_HIGH, 
            .operation = OP_GRID_ALERT, 
            .payload = "GRID_FAULT"
        };
        send_message(socket, &alert);
    }
    ```

  - Measure response latency and throughput
  - Notes: Validates energy sector reliability

- [ ] **Automated Test Framework** (4 days, by June 16, 2025)
  - Create test harness for kernel components
  - Implement CI/CD pipeline with GitHub Actions
  - Add code coverage reporting
  - Notes: Improves development workflow

## Documentation

### High Priority

- [x] **Basic README**
  - Added project overview
  - Added build instructions
  - Added running instructions

- [ ] **Enhance README.md**
  - Add detailed build instructions
  - Include troubleshooting guide
  - Document security features
  - Notes: Essential for onboarding

- [ ] **API Documentation**
  - Document scheduler_ops_t, storage_ops_t interfaces
  - Create usage examples
  - Add diagrams for component interactions
  - Notes: Facilitates development

### Medium Priority

- [ ] **Architecture Documentation**
  - Update docs/architecture.md with microkernel principles
  - Document component interactions
  - Explain security boundaries
  - Notes: Important for contributors

- [ ] **Security Documentation**
  - Update docs/security.md with threat model
  - Document security features and validation
  - Include secure coding standards
  - Notes: Critical for infrastructure use

## Community and Project Management

### Medium Priority

- [ ] **Project Roadmap**
  - Create milestone plan for next 6 months
  - Document vision and goals
  - Define success metrics
  - Notes: Guides development priorities

- [ ] **Contribution Guidelines**
  - Create CONTRIBUTING.md
  - Define code review process
  - Document coding standards
  - Notes: Facilitates community involvement

### Low Priority

- [ ] **GitHub Repository Enhancement**
  - Add issue templates
  - Configure GitHub Actions for CI
  - Add badges to README
  - Notes: Improves project visibility

- [ ] **Community Outreach**
  - Share progress on social media
  - Prepare presentations for FOSDEM/OSS conferences
  - Notes: Builds community

## Future Enhancements

### Low Priority

- [ ] **Distributed Operation**
  - Implement node health monitoring
  - Add distributed consensus for multi-node systems
  - Support failover for critical functions
  - Notes: For multi-node systems

- [ ] **Performance Metrics System**
  - Add instrumentation points throughout kernel
  - Create performance dashboard
  - Implement automatic bottleneck detection
  - Notes: Enables continuous optimization

## Notes

- **Energy Focus**: All components must prioritize grid operations (OP_GRID_ALERT)
- **Security**: Defense-in-depth approach with multiple validation layers
- **Reliability**: Zero downtime requirement for hot-swapping and updates
- **Composability**: All components must implement clean interfaces for swapping
