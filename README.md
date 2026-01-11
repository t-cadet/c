# Overview

A collection of standalone header-only C libraries for use in my libc-free projects.

## Contents

(see the top of each header file for more details)

* **linux.h**: Cross-architecture Linux API

## Getting Started

Get a header file:
```bash
mkdir c
wget -O c/linux.h https://raw.githubusercontent.com/t-cadet/c/refs/heads/main/linux.h 
```

Include it in your project:
```c
#include "c/linux.h" // use as header file

#define C_LINUX_IMPLEMENTATION
#include "c/linux.h" // use as implementation file
```

## Examples

See `*_demo` functions in `*_demo.c` files.

---

To run a demo file:
```bash
git clone https://github.com/t-cadet/c.git
cd c
# run the command at the top of the demo file, for example:
clang -nostdlib -static -fuse-ld=lld -ffreestanding -o linux_demo linux_demo.c -e main && ./linux_demo
```

