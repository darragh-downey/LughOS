# LughOS Architecture Overview

## Purpose

LughOS is a microkernel operating system designed for critical infrastructure, emphasizing modularity, security, zero downtime, and real-time performance. The MVP demonstrates a minimal system with hot-swappable schedulers, prioritized messaging, and transactional updates.

## Components

### Microkernel

Handles task management, IPC (via NNG), and memory allocation.
Minimal TCB for security, running in privileged mode.

### Scheduler Service

User-space service implementing scheduling algorithms (e.g., round-robin, priority-based).
Supports hot-swapping with zero downtime using Tanenbaum’s live update mechanism.

### Storage Service

Manages persistent data with transactional updates and rollback.
Uses a simple key-value store for MVP.

### IPC Framework

NNG-based messaging with priority levels (HIGH, MEDIUM, LOW) for critical operations.

### Reincarnation Server

Monitors services and restarts failed components, ensuring fault tolerance.

## Interactions

Kernel-Services: The kernel communicates with services (e.g., scheduler, storage) via NNG inproc sockets.
Scheduler Swapping: The reincarnation server coordinates state transfer and message redirection during swaps.
Transactional Updates: All critical operations (e.g., scheduler swaps, storage writes) are logged for rollback.
Prioritized Messaging: High-priority messages (e.g., scheduling commands) are processed first to ensure real-time performance.

## Design Principles

Modularity: Services run in user space, isolated via memory protection.
Zero Downtime: Hot-swappable components and buffered messages prevent interruptions.
Security: Least privilege and memory safety minimize vulnerabilities.
Reliability: Transactional updates and reincarnation server ensure fault tolerance.

## MVP Scope

Single-node system running on x86/ARM (QEMU).
One scheduler (round-robin) with swap to priority-based scheduler.
Basic storage service with transactional logging.
NNG-based IPC with prioritized messaging.
Target use case: Real-time task management for energy grid sensors.

## Future Extensions

Distributed operation for multi-node infrastructure.
Formal verification for high-assurance systems.
Component marketplace for developer ecosystem.
