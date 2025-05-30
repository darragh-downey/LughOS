# LughOS Makefile for x86 and ARM on Fedora 42 with hardening

X86_CC = i686-elf-gcc
X86_LD = i686-elf-ld
ARM_CC = arm-none-eabi-gcc
ARM_LD = arm-none-eabi-ld
# Security-focused compiler flags per SEI CERT and NASA Power of Ten
CFLAGS = -ffreestanding -nostdlib -Wall -Wextra -Werror -Wformat=2 -Wformat-security \
         -fPIE -fno-strict-aliasing \
         -fdata-sections -ffunction-sections \
         -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wredundant-decls \
         -Wconversion -Wno-unused-parameter \
         -DDEBUG -D_FORTIFY_SOURCE=2 -Iinclude -Ilib/nng/include -O2
# commented out for the time being -fstack-protector-strong
# Comment out for now as it's incompatible with our cross-compiler
# LIBS = lib/nng/lib/libnng.a
LIBS =
X86_LDFLAGS = -T kernel/linker_x86.ld
ARM_LDFLAGS = -T kernel/linker_arm.ld

# Updated kernel source paths for new structure
KERNEL_SRC = \
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
  kernel/sched/task.c

SCHEDULER_SRC = services/scheduler/round_robin.c services/scheduler/utils.c
STORAGE_SRC = services/storage/storage.c services/storage/transactions.c
X86_BOOT_SRC = kernel/arch/x86/boot_x86.S kernel/arch/x86/enter_user_mode.S
ARM_BOOT_SRC = kernel/arch/arm/boot_arm.S

X86_OBJS = $(KERNEL_SRC:.c=.x86.o) $(SCHEDULER_SRC:.c=.x86.o) $(STORAGE_SRC:.c=.x86.o) $(X86_BOOT_SRC:.S=.x86.o)
ARM_OBJS = $(KERNEL_SRC:.c=.arm.o) $(SCHEDULER_SRC:.c=.arm.o) $(STORAGE_SRC:.c=.arm.o) $(ARM_BOOT_SRC:.S=.arm.o)

X86_OUT = build/x86
ARM_OUT = build/arm
X86_BIN = $(X86_OUT)/lughos.bin
ARM_BIN = $(ARM_OUT)/lughos.bin

all: x86 arm

x86: $(X86_BIN)

arm: $(ARM_BIN)

$(X86_BIN): $(X86_OBJS)
	@mkdir -p $(X86_OUT)
	$(X86_LD) $(X86_LDFLAGS) -o $@ $^ $(LIBS)
	@echo "Built x86 kernel: $@"
	@echo "Checking binary for security issues..."
	@objdump -d $@ | grep -i "mov.*esp.*eax" || echo "No stack exec vulnerabilities found"

$(ARM_BIN): $(ARM_OBJS)
	@mkdir -p $(ARM_OUT)
	$(ARM_LD) $(ARM_LDFLAGS) -o $@ $^ $(LIBS)
	@echo "Built ARM kernel: $@"
	@echo "Checking binary for security issues..."
	@objdump -d $@ | grep -i "mov.*sp.*r" || echo "No stack exec vulnerabilities found"

%.x86.o: %.c 
	$(X86_CC) $(CFLAGS) -c $< -o $@ 

%.x86.o: %.S
	$(X86_CC) $(CFLAGS) -c $< -o $@

%.arm.o: %.c
	$(ARM_CC) $(CFLAGS) -c $< -o $@

%.arm.o: %.S
	$(ARM_CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build *.o kernel/*.o kernel/*/*.o kernel/*/*/*.o services/*/*.o

run: x86
	@echo "Running LughOS x86 in QEMU..."
	@./scripts/test_x86.sh

debug: x86
	@echo "Running LughOS x86 in QEMU with GDB debugging..."
	@./scripts/run_x86.sh

analyze:
	cppcheck --enable=all --error-exitcode=1 --suppress=missingIncludeSystem \
	kernel/*.c services/*/*.c

security-check:
	@echo "Performing security checks..."
	@grep -r "strcpy\|strcat\|sprintf\|gets" kernel/ services/ || echo "No unsafe string functions found"
	@objdump -d $(X86_BIN) | grep -i "jmp.*esp" || echo "No stack execution vulnerabilities detected"
	@echo "Running static analysis with cppcheck..."
	@$(MAKE) analyze

.PHONY: all x86 arm clean run debug analyze security-check