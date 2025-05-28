# LughOS

LughOS is a microkernel OS for critical infrastructure, emphasizing security, modularity, and zero downtime.
Setup (Fedora 42)

## Install dependencies

```bash
sudo dnf install -y gcc-arm-none-eabi binutils-arm-none-eabi qemu-system-x86 qemu-system-arm cmake make git checkout
# Install i686-elf-gcc (see instructions below)
# Install NNG
cd ~/Downloads
git clone https://github.com/nanomsg/nng.git
cd nng
mkdir build && cd build
cmake -G "Ninja" ..
ninja
sudo ninja install
export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig:$PKG_CONFIG_PATH
```

### Build

```bash
make x86 arm
```

### Run in QEMU

```bash
./scripts/run_x86.sh
./scripts/run_arm.sh
```

## Current MVP Status

Basic kernel boots on x86 and ARM, printing a boot message.
