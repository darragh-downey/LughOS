#!/bin/bash
qemu-system-arm -M versatilepb -kernel build/arm/lughos.bin -nographic -serial mon:stdio -s -S &
arm-none-eabi-gdb build/arm/lughos.bin -ex "target remote localhost:1234" -ex "break kmain" -ex "continue"
