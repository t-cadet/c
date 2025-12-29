// clang -nostdlib -static -fuse-ld=lld -ffreestanding -o linux_demo linux_demo.c -e main && ./linux_demo
// 
// Cross-compilation:
// 
// clang --target=aarch64-linux-gnu -nostdlib -static -fuse-ld=lld -ffreestanding -o linux_demo linux_demo.c -e main
// qemu-aarch64 ./linux_demo
// 
// clang --target=riscv32-linux-gnu -nostdlib -static -fuse-ld=lld -ffreestanding -o linux_demo linux_demo.c -e main
// qemu-riscv32 ./linux_demo
// 
// clang --target=i386-linux-gnu -nostdlib -static -fuse-ld=lld -ffreestanding -o linux_demo linux_demo.c -e main
// qemu-i386 ./linux_demo

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

char *hello = "Hello from " ARCHNAME ", libc-free world!\n";

// Constants
#define AT_FDCWD  -100
#define O_RDONLY    00
#define O_WRONLY    01
#define O_RDWR      02
#define O_CREAT     0100
#define O_TRUNC     01000
#define SEEK_SET    0

#define true        1
#define false       0

#define NULL 0

#define STDOUT 1
#define STDERR 2

#define EINVAL 22

#define LO32(x) (x & 0xFFFFFFFF)
#define HI32(x) (x >> 32)

// Wrappers
long close(int fd) {
  return Syscall1_linux(NR_close_linux, fd, 0);
}

long read(int fd, void *buf, unsigned long count) {
  return Syscall3_linux(NR_read_linux, fd, buf, count, 0);
}

long write(int fd, const void *buf, unsigned long count) {
  return Syscall3_linux(NR_write_linux, fd, buf, count, 0);
}

int unlink(const char* path) {
  return Syscall3_linux(NR_unlinkat_linux, AT_FDCWD, path, 0, 0);
}

int open(const char *path, int flags, int mode) {
  return Syscall4_linux(NR_openat_linux, AT_FDCWD, path, flags, mode, 0);
}

int fsync(int fd) {
  return Syscall1_linux(NR_fsync_linux, fd, 0);
}

long llseek(int fd, long long offset, int whence) {
  #if (defined(__riscv) && __riscv_xlen == 32)
    // TODO: There seems to be an issue with the
    // syscall number data coming from glibc, it
    // seems there is no llseek but only _llseek
    #undef NR__llseek_linux
    #define NR__llseek_linux NR_llseek_linux
  #endif
  #if (defined(__riscv) && __riscv_xlen == 32) || defined(__i386__) || defined(__arm__)
    long long result;
    long ret = Syscall5_linux(
        NR__llseek_linux,
        fd,
        HI32(offset),
        LO32(offset),
        &result,
        whence,
        0
    );
    if (ret < 0) return ret;
    return result;
#else
  return Syscall3_linux(NR_lseek_linux, fd, offset, whence, 0);
#endif
}

void exit(int status) {
  Syscall1_linux(NR_exit_linux, status, 0);
  __builtin_unreachable();
}

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
    write(STDERR, message, Size_chars(message));
    exit(1);
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
  long count = write(fd, data, stringSize);
  Assert(count == stringSize); 
  return count;
}

void Cleanup(const char *f1, const char *f2, int fd1, int fd2) {
    if (fd1 >= 0) close(fd1);
    if (fd2 >= 0) close(fd2);
    unlink(f1);
    unlink(f2);
}

void Syscall6_test() {
  const char *srcname = "test_src.txt";
  const char *dstname = "test_dst.txt";
  const char *content = "abcdefghijHello, Syscall6 World!";
  unsigned long len = Size_chars(content);

  int fdIn = open(srcname, O_RDWR | O_CREAT | O_TRUNC, 0644);
  Assert(fdIn > 0);
  Print(fdIn, content);

  fsync(fdIn);

  int fdOut = open(dstname, O_RDWR | O_CREAT | O_TRUNC, 0644);
  Assert(fdOut > 0);
  Print(fdOut, "abcdefghij");

  {
    Print(STDERR, "Test A: Sending invalid flags (expecting failure)...\n");
    long long offIn = 10;
    long long offOut = 10;
    long retFail = Syscall6_linux(
        NR_copy_file_range_linux,
        fdIn,
        &offIn,
        fdOut,
        &offOut,
        len - 10,
        0xFF, // no flags is supported so this should fail
        0     // ret2 pointer
    );
    if (retFail == -EINVAL) {
        Print(STDERR, "Test A: Passed: Kernel received flag 0xFF and rejected it.\n");
    } else if (retFail >= 0) {
        Print(STDERR, "Test A: FAILED: Syscall succeeded! Arg 6 was likely lost/zeroed.\n");
        exit(1);
    } else {
        Print(STDERR, "Test A: warning: Failed with unexpected error: ");
        exit(retFail);
    }
  }

  {
    long long offIn = 10;
    long long offOut = 10;

    Print(STDERR, "Test B: Sending valid flags (expecting success)...\n");
    long ret = Syscall6_linux(
        NR_copy_file_range_linux,
        fdIn,
        &offIn,
        fdOut,
        &offOut,
        len - 10,
        0,                        // Arg 6: flags
        0                         // ret2 pointer
    );

    if (ret < 0) {
        Print(STDERR, "Test B: Syscall failed\n");
        Cleanup(srcname, dstname, fdIn, fdOut);
        exit(1);
    } else if (ret != len - 10) {
        Print(STDERR, "Test B: Syscall succeeded but copied incomplete bytes\n");
    } else {
        Print(STDERR, "Test B: Success!\n");
    }

    char buf[100];
    llseek(fdOut, 0, SEEK_SET);
  
    long readLen = read(fdOut, buf, sizeof(buf));
    if (readLen > 0) {
        buf[readLen] = 0;
        if (Eq_chars(buf, content)) {
            Print(STDERR, "Test B: Content check passed.\n");
        } else {
            Print(STDERR, "Test B: Content check FAILED.\n");

            Print(STDERR, "Test B: Expected: ");
            Print(STDERR, content);
            Print(STDERR, "\n");

            Print(STDERR, "Test B: Got: ");
            Print(STDERR, buf);
            Print(STDERR, "\n");
        }
    }
  }

  Cleanup(srcname, dstname, fdIn, fdOut);
}

void SyscallWrapper_demo() {
  // TODO: use linux.h wrappers
  #define write_linux write
  long ret = write_linux(STDOUT, hello, Size_chars(hello));
  Assert(ret == Size_chars(hello));
}

void SyscallN_demo() {
  long ret = Syscall3_linux(NR_write_linux, // syscall number
               STDOUT,                      // arg1
               hello,                       // arg2
               Size_chars(hello),           // arg3
               NULL);                       // ret2 pointer (some syscalls have 2 return values e.g., pipe() on MIPS/Alpha).
  Assert(ret == Size_chars(hello));
}

int main(void) {
  SyscallWrapper_demo();
  SyscallN_demo();
  // TODO: CompatibilityWrappersForUnimplementedSyscalls_demo();

  Print(STDOUT, "\n");

  Syscall6_test();
  exit(42);

  return 0;
}
