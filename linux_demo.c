// clang -O0 -nostdlib -static -fuse-ld=lld -ffreestanding -o linux_demo linux_demo.c -e main && ./linux_demo
// 
// Cross-compilation:
// 
// clang --target=aarch64-linux-gnu -nostdlib -static -fuse-ld=lld -ffreestanding -o linux_demo linux_demo.c -e main && qemu-aarch64 ./linux_demo
// clang --target=riscv64-linux-gnu -nostdlib -static -fuse-ld=lld -ffreestanding -o linux_demo linux_demo.c -e main && qemu-riscv64 ./linux_demo
// 
// clang --target=i386-linux-gnu -nostdlib -static -fuse-ld=lld -ffreestanding -o linux_demo linux_demo.c -e main && qemu-i386 ./linux_demo
// clang --target=arm-linux-gnueabihf -nostdlib -static -fuse-ld=lld -ffreestanding -o linux_demo linux_demo.c -e main && qemu-arm ./linux_demo
// clang --target=riscv32-linux-gnu -nostdlib -static -fuse-ld=lld -ffreestanding -o linux_demo linux_demo.c -e main && qemu-riscv32 ./linux_demo
// 

#define C_LINUX_IMPLEMENTATION
#include "linux.h"

#if defined(__x86_64__)
  #define ARCHNAME "x86_64"
#elif defined(__aarch64__)
  #define ARCHNAME "arm64"
#elif defined(__riscv) && (__riscv_xlen == 64)
  #define ARCHNAME "riscv64"
#elif defined(__i386__)
  #define ARCHNAME "x86_32"
#elif defined(__arm__)
  #define ARCHNAME "arm32"
#elif defined(__riscv) && (__riscv_xlen == 32)
  #define ARCHNAME "riscv32"
#endif

char *hello = "Demo: Hello from " ARCHNAME ", libc-free world!\n";

#define true        1
#define false       0

#define NULL 0

// Helpers
unsigned long Size_chars(const char* chars) {
  unsigned long size = 0;
  while (*chars++) {
    ++size;
  }
  return size;
}

void _Assert(int condition, const char* message) {
  if (!condition) {
    write_linux(STDERR_FILENO_linux, message, Size_chars(message));
    exit_linux(1);
  }
}

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define Assert(condition) _Assert((condition), ("FATAL: Assert failed at " __FILE__ ":" TOSTRING(__LINE__) "\n"))

int Eq_chars(const char* a, const char* b) {
  Assert(a && b);
  int eq = true;
  if (a != b) {
    do {
      eq = *a++ == *b++;
    } while (eq && *a && *b);
  }
  return eq;
}

long Print(int fd, const char* data) {
  unsigned long stringSize = Size_chars(data);
  long count = write_linux(fd, data, stringSize);
  Assert(count == stringSize); 
  return count;
}

void Cleanup(const char *f1, const char *f2, int fd1, int fd2) {
    if (fd1 >= 0) close_linux(fd1);
    if (fd2 >= 0) close_linux(fd2);
    unlink_linux(f1);
    unlink_linux(f2);
}

void Syscall6_test() {
  const char *srcname = "test_src.txt";
  const char *dstname = "test_dst.txt";
  const char *content = "abcdefghijHello, Syscall6 World!";
  unsigned long len = Size_chars(content);

  int fdIn = open_linux(srcname, O_RDWR_linux | O_CREAT_linux | O_TRUNC_linux, 0644);
  Assert(fdIn > 0);
  Print(fdIn, content);

  fsync_linux(fdIn);

  int fdOut = open_linux(dstname, O_RDWR_linux | O_CREAT_linux | O_TRUNC_linux, 0644);
  Assert(fdOut > 0);
  Print(fdOut, "abcdefghij");

  {
    Print(STDERR_FILENO_linux, "Test A: Sending invalid flags (expecting failure)...\n");
    long long offIn = 10;
    long long offOut = 10;
    long retFail = copy_file_range_linux(
        fdIn,
        &offIn,
        fdOut,
        &offOut,
        len - 10,
        0xFF // no flags is supported so this should fail
    );
    if (retFail == -EINVAL_linux) {
        Print(STDERR_FILENO_linux, "Test A: Passed: Kernel received flag 0xFF and rejected it.\n");
    } else if (retFail >= 0) {
        Print(STDERR_FILENO_linux, "Test A: FAILED: Syscall succeeded! Arg 6 was likely lost/zeroed.\n");
        exit_linux(1);
    } else {
        Print(STDERR_FILENO_linux, "Test A: warning: Failed with unexpected error: ");
        exit_linux(retFail);
    }
  }

  {
    long long offIn = 10;
    long long offOut = 10;

    Print(STDERR_FILENO_linux, "Test B: Sending valid flags (expecting success)...\n");
    long ret = copy_file_range_linux(
        fdIn,
        &offIn,
        fdOut,
        &offOut,
        len - 10,
        0                        // Arg 6: flags
    );

    if (ret < 0) {
        Print(STDERR_FILENO_linux, "Test B: Syscall failed\n");
        Cleanup(srcname, dstname, fdIn, fdOut);
        exit_linux(1);
    } else if (ret != len - 10) {
        Print(STDERR_FILENO_linux, "Test B: Syscall succeeded but copied incomplete bytes\n");
    } else {
        Print(STDERR_FILENO_linux, "Test B: Success!\n");
    }

    char buf[100];
    long long result = 0;
    llseek_linux(fdOut, 0, &result, SEEK_SET_linux);
  
    long readLen = read_linux(fdOut, buf, sizeof(buf));
    if (readLen > 0) {
        buf[readLen] = 0;
        if (Eq_chars(buf, content)) {
            Print(STDERR_FILENO_linux, "Test B: Content check passed.\n");
        } else {
            Print(STDERR_FILENO_linux, "Test B: Content check FAILED.\n");

            Print(STDERR_FILENO_linux, "Test B: Expected: ");
            Print(STDERR_FILENO_linux, content);
            Print(STDERR_FILENO_linux, "\n");

            Print(STDERR_FILENO_linux, "Test B: Got: ");
            Print(STDERR_FILENO_linux, buf);
            Print(STDERR_FILENO_linux, "\n");
        }
    }
  }

  Cleanup(srcname, dstname, fdIn, fdOut);
}

void SyscallWrapper_demo() {
  long ret = write_linux(STDOUT_FILENO_linux, hello, Size_chars(hello));
  Assert(ret == Size_chars(hello));
}

void SyscallN_demo() {
  long ret = Syscall3_linux(NR_write_linux, // syscall number
               STDOUT_FILENO_linux,                      // arg1
               hello,                       // arg2
               Size_chars(hello),           // arg3
               NULL);                       // ret2 pointer (some syscalls have 2 return values e.g., pipe() on MIPS/Alpha).
  Assert(ret == Size_chars(hello));
}

int main(void) {
  SyscallWrapper_demo();
  SyscallN_demo();

  Print(STDOUT_FILENO_linux, "\n");

  Syscall6_test();
  exit_linux(42);
  Syscall6_test();

  return 0;
}
