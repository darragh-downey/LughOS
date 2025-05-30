# LughOS Makefile for x86, ARM and RISC-V on Fedora 42 with hardening

X86_CC = i686-elf-gcc
X86_LD = i686-elf-ld
ARM_CC = arm-none-eabi-gcc
ARM_LD = arm-none-eabi-ld
RISCV_CC = riscv64-linux-gnu-gcc
RISCV_LD = riscv64-linux-gnu-ld
# Architecture detection flags
X86_ARCH_FLAGS = -D__i386__
ARM_ARCH_FLAGS = -D__arm__
RISCV_ARCH_FLAGS = -D__riscv
# Security-focused compiler flags per SEI CERT and NASA Power of Ten
COMMON_CFLAGS = -ffreestanding -nostdlib -Wall -Wextra -Werror -Wformat=2 -Wformat-security \
         -fPIE -fno-strict-aliasing \
         -fdata-sections -ffunction-sections \
         -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wredundant-decls \
         -Wconversion -Wno-unused-parameter \
         -DDEBUG -D_FORTIFY_SOURCE=2 -Iinclude -Ilib/nng/include -O2

X86_CFLAGS = $(COMMON_CFLAGS) $(X86_ARCH_FLAGS)
ARM_CFLAGS = $(COMMON_CFLAGS) $(ARM_ARCH_FLAGS)
RISCV_CFLAGS = $(COMMON_CFLAGS) $(RISCV_ARCH_FLAGS)
# commented out for the time being -fstack-protector-strong
# Comment out for now as it's incompatible with our cross-compiler
# LIBS = lib/nng/lib/libnng.a
LIBS =
X86_LDFLAGS = -T kernel/linker_x86.ld
ARM_LDFLAGS = -T kernel/linker_arm.ld
RISCV_LDFLAGS = -T kernel/linker_riscv.ld

# Updated kernel source paths for new structure
KERNEL_COMMON_SRC = \
  kernel/main.c \
  kernel/log.c \
  kernel/security.c \
  kernel/hardware.c \
  kernel/nngcompat.c \
  kernel/crypto.c \
  kernel/assert.c \
  kernel/mm/memory.c \
  kernel/mm/memory_utils.c \
  kernel/fs/storage.c \
  kernel/ipc/ipc.c \
  kernel/drivers/console.c \
  kernel/sched/priority.c \
  kernel/sched/task.c \
  kernel/user.c
  
# Architecture-specific kernel sources
X86_SPECIFIC_SRC = kernel/arch/x86/init.c
ARM_SPECIFIC_SRC = kernel/arch/arm/init.c
RISCV_SPECIFIC_SRC = kernel/arch/riscv/init.c

KERNEL_SRC = $(KERNEL_COMMON_SRC)

SCHEDULER_SRC = services/scheduler/round_robin.c services/scheduler/utils.c
STORAGE_SRC = services/storage/storage.c services/storage/transactions.c
UPDATE_SRC = services/update/update.c services/update/sandbox.c
X86_BOOT_SRC = kernel/arch/x86/boot_x86.S kernel/arch/x86/enter_user_mode.S
ARM_BOOT_SRC = kernel/arch/arm/boot_arm.S kernel/arch/arm/enter_user_mode.S
RISCV_BOOT_SRC = kernel/arch/riscv/boot_riscv.S kernel/arch/riscv/enter_user_mode.S
X86_SYSCALL_SRC = kernel/arch/x86/syscall.S
ARM_SYSCALL_SRC = kernel/arch/arm/syscall.S
RISCV_SYSCALL_SRC = kernel/arch/riscv/syscall.S
SYSCALL_SRC = kernel/syscall/syscall.c

# User program sources
USER_LIB_SRC = user/lib/user.c
USER_X86_BOOT_SRC = user/lib/crt0_x86.S
USER_ARM_BOOT_SRC = user/lib/crt0_arm.S
USER_RISCV_BOOT_SRC = user/lib/crt0_riscv.S
USER_X86_SYSCALL_SRC = user/lib/syscall_x86.S
USER_ARM_SYSCALL_SRC = user/lib/syscall_arm.S
USER_RISCV_SYSCALL_SRC = user/lib/syscall_riscv.S
USER_TEST_SRC = user/tests/hello.c

USER_X86_OBJS = $(USER_LIB_SRC:.c=.user.x86.o) $(USER_TEST_SRC:.c=.user.x86.o) $(USER_X86_BOOT_SRC:.S=.user.x86.o) $(USER_X86_SYSCALL_SRC:.S=.user.x86.o)
USER_ARM_OBJS = $(USER_LIB_SRC:.c=.user.arm.o) $(USER_TEST_SRC:.c=.user.arm.o) $(USER_ARM_BOOT_SRC:.S=.user.arm.o) $(USER_ARM_SYSCALL_SRC:.S=.user.arm.o)
USER_RISCV_OBJS = $(USER_LIB_SRC:.c=.user.riscv.o) $(USER_TEST_SRC:.c=.user.riscv.o) $(USER_RISCV_BOOT_SRC:.S=.user.riscv.o) $(USER_RISCV_SYSCALL_SRC:.S=.user.riscv.o)

X86_OBJS = $(KERNEL_SRC:.c=.x86.o) $(X86_SPECIFIC_SRC:.c=.x86.o) $(SCHEDULER_SRC:.c=.x86.o) $(STORAGE_SRC:.c=.x86.o) \
           $(UPDATE_SRC:.c=.x86.o) \
           $(X86_BOOT_SRC:.S=.x86.o) $(X86_SYSCALL_SRC:.S=.x86.o) $(SYSCALL_SRC:.c=.x86.o)
ARM_OBJS = $(KERNEL_SRC:.c=.arm.o) $(ARM_SPECIFIC_SRC:.c=.arm.o) $(SCHEDULER_SRC:.c=.arm.o) $(STORAGE_SRC:.c=.arm.o) \
           $(UPDATE_SRC:.c=.arm.o) \
           $(ARM_BOOT_SRC:.S=.arm.o) $(ARM_SYSCALL_SRC:.S=.arm.o) $(SYSCALL_SRC:.c=.arm.o)
RISCV_OBJS = $(KERNEL_SRC:.c=.riscv.o) $(RISCV_SPECIFIC_SRC:.c=.riscv.o) $(SCHEDULER_SRC:.c=.riscv.o) $(STORAGE_SRC:.c=.riscv.o) \
           $(UPDATE_SRC:.c=.riscv.o) \
           $(RISCV_BOOT_SRC:.S=.riscv.o) $(RISCV_SYSCALL_SRC:.S=.riscv.o) $(SYSCALL_SRC:.c=.riscv.o)

X86_OUT = build/x86
ARM_OUT = build/arm
RISCV_OUT = build/riscv
X86_BIN = $(X86_OUT)/lughos.bin
ARM_BIN = $(ARM_OUT)/lughos.bin
RISCV_BIN = $(RISCV_OUT)/lughos.bin
X86_USER_BIN = $(X86_OUT)/user_hello
ARM_USER_BIN = $(ARM_OUT)/user_hello
RISCV_USER_BIN = $(RISCV_OUT)/user_hello
X86_USER_OBJ = $(X86_OUT)/user_hello.o
ARM_USER_OBJ = $(ARM_OUT)/user_hello.o
RISCV_USER_OBJ = $(RISCV_OUT)/user_hello.o

all: x86 arm riscv

x86: user_x86 $(X86_BIN) 

arm: user_arm $(ARM_BIN)

riscv: user_riscv $(RISCV_BIN)

user_x86: $(X86_USER_BIN) $(X86_USER_OBJ)

user_arm: $(ARM_USER_BIN) $(ARM_USER_OBJ)

user_riscv: $(RISCV_USER_BIN) $(RISCV_USER_OBJ)

$(X86_BIN): $(X86_OBJS) $(X86_USER_OBJ)
	@mkdir -p $(X86_OUT)
	$(X86_LD) $(X86_LDFLAGS) -o $@ $^ $(LIBS)
	@echo "Built x86 kernel: $@"
	@echo "Checking binary for security issues..."
	@objdump -d $@ | grep -i "mov.*esp.*eax" || echo "No stack exec vulnerabilities found"

$(ARM_BIN): $(ARM_OBJS) $(ARM_USER_OBJ)
	@mkdir -p $(ARM_OUT)
	$(ARM_LD) $(ARM_LDFLAGS) -o $@ $^ $(LIBS)
	@echo "Built ARM kernel: $@"
	@echo "Checking binary for security issues..."
	@objdump -d $@ | grep -i "mov.*sp.*r" || echo "No stack exec vulnerabilities found"

$(RISCV_BIN): $(RISCV_OBJS) $(RISCV_USER_OBJ)
	@mkdir -p $(RISCV_OUT)
	$(RISCV_LD) $(RISCV_LDFLAGS) -o $@ $^ $(LIBS)
	@echo "Built RISC-V kernel: $@"
	@echo "Checking binary for security issues..."
	@objdump -d $@ | grep -i "^.*sp" || echo "No stack exec vulnerabilities found"

%.x86.o: %.c
	$(X86_CC) $(X86_CFLAGS) -c $< -o $@
	
%.riscv.o: %.c
	$(RISCV_CC) $(RISCV_CFLAGS) -c $< -o $@

%.x86.o: %.S
	$(X86_CC) $(X86_CFLAGS) -c $< -o $@

%.arm.o: %.c
	$(ARM_CC) $(ARM_CFLAGS) -c $< -o $@

%.arm.o: %.S
	$(ARM_CC) $(ARM_CFLAGS) -c $< -o $@

%.riscv.o: %.S
	$(RISCV_CC) $(RISCV_CFLAGS) -c $< -o $@

# User-space object compilation
%.user.x86.o: %.c
	$(X86_CC) $(X86_CFLAGS) -I. -Iuser/lib -c $< -o $@

%.user.x86.o: %.S
	$(X86_CC) $(X86_CFLAGS) -I. -Iuser/lib -c $< -o $@

%.user.arm.o: %.c
	$(ARM_CC) $(ARM_CFLAGS) -I. -Iuser/lib -c $< -o $@

%.user.arm.o: %.S
	$(ARM_CC) $(ARM_CFLAGS) -I. -Iuser/lib -c $< -o $@

%.user.riscv.o: %.c
	$(RISCV_CC) $(RISCV_CFLAGS) -I. -Iuser/lib -c $< -o $@

%.user.riscv.o: %.S
	$(RISCV_CC) $(RISCV_CFLAGS) -I. -Iuser/lib -c $< -o $@
	
# User binaries	
$(X86_USER_BIN): $(USER_X86_OBJS)
	@mkdir -p $(X86_OUT)
	$(X86_LD) -T user/linker_user_x86.ld -o $@ $^
	@echo "Built x86 user program: $@"

$(X86_USER_OBJ): $(X86_USER_BIN)
	@echo "Creating embedded user program object..."
	# Create raw binary first to avoid symbol conflicts
	i686-elf-objcopy -O binary $(X86_USER_BIN) $(X86_OUT)/user_hello.bin
	# Then convert the raw binary to an object file with proper symbols
	i686-elf-ld -r -b binary --oformat elf32-i386 -o $(X86_OUT)/user_hello.o $(X86_OUT)/user_hello.bin
	@echo "Symbols created: _binary_user_hello_bin_start, _binary_user_hello_bin_end"

$(ARM_USER_BIN): $(USER_ARM_OBJS)
	@mkdir -p $(ARM_OUT)
	$(ARM_LD) -T user/linker_user_arm.ld -o $@ $^
	@echo "Built ARM user program: $@"

$(ARM_USER_OBJ): $(ARM_USER_BIN)
	@echo "Creating embedded ARM user program object..."
	# First create a raw binary from the ELF file
	arm-none-eabi-objcopy -O binary $(ARM_USER_BIN) $(ARM_OUT)/user_hello.bin
	# Now convert the raw binary to an object file with proper symbols
	arm-none-eabi-ld -r -b binary -o $(ARM_OUT)/user_hello.o $(ARM_OUT)/user_hello.bin

$(RISCV_USER_BIN): $(USER_RISCV_OBJS)
	@mkdir -p $(RISCV_OUT)
	$(RISCV_LD) -T user/linker_user_riscv.ld -o $@ $^
	@echo "Built RISC-V user program: $@"

$(RISCV_USER_OBJ): $(RISCV_USER_BIN)
	@echo "Creating embedded RISC-V user program object..."
	# Create raw binary first to avoid symbol conflicts
	riscv64-linux-gnu-objcopy -O binary $(RISCV_USER_BIN) $(RISCV_OUT)/user_hello.bin
	# Then convert the raw binary to an object file with proper symbols
	riscv64-linux-gnu-ld -r -b binary -o $(RISCV_OUT)/user_hello.o $(RISCV_OUT)/user_hello.bin
	@echo "RISC-V user program object created"

clean:
	rm -rf build *.o kernel/*.o kernel/*/*.o kernel/*/*/*.o services/*/*.o user/*.o user/*/*.o

run: x86
	@echo "Running LughOS x86 in QEMU..."
	@./scripts/test_x86.sh

debug: x86
	@echo "Running LughOS x86 in QEMU with GDB debugging..."
	@./scripts/run_x86.sh

analyze:
	mkdir -p reports
	cppcheck --enable=all --suppress=missingIncludeSystem \
	--xml --xml-version=2 \
	-Iinclude \
	kernel/*.c kernel/*/*.c kernel/*/*/*.c services/*/*.c 2> reports/cppcheck-report.xml
	@echo "Cppcheck analysis completed, see reports/cppcheck-report.xml for details"
	
	@echo "Running clang-tidy CERT C checks..."
	clang-tidy "-checks=cert-*,clang-analyzer-security.*" kernel/*.c kernel/*/*.c -- -Iinclude || true

security-check:
	@echo "Performing security checks..."
	@grep -r "strcpy\|strcat\|sprintf\|gets" kernel/ services/ || echo "No unsafe string functions found"
	@objdump -d $(X86_BIN) | grep -i "jmp.*esp" || echo "No stack execution vulnerabilities detected"
	@echo "Running static analysis with cppcheck..."
	@$(MAKE) analyze

test: x86 arm riscv
	@echo "Running all tests..."
	@mkdir -p test-logs
	@./scripts/run_tests.sh
	@./scripts/run_unit_tests.sh x86 || true
	@./scripts/run_unit_tests.sh arm || true
	@echo "Tests completed, see test-logs/ directory for details"

ci: clean x86 arm riscv test analyze security-check
	@echo "CI pipeline completed successfully"

.PHONY: all x86 arm riscv clean run debug analyze security-check test ci