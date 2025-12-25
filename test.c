// clang -nostdlib -static -o test test.c -e main && ./test

#include "linux.h"

int STDOUT = 1;

long write_linux(int fd, const void *buf, unsigned long count) {
  long out;
    __asm__ volatile (
      "syscall"
      : "=a" (out)
      : "a" (NR_write_linux), "D" (fd), "S" (buf), "d" (count)
      : "rcx", "r11", "memory"
  );
  return out;
}

void exit_linux(int status) {
  __asm__ volatile (
      "syscall"
      :
      : "a" (NR_exit_linux), "D" (status)
      : "rcx", "r11", "memory"
  );
  __builtin_unreachable();
}

unsigned long Size_chars(char* chars) {
  unsigned long size = 0;
  while (*chars++) {
    ++size;
  }
  return size;
}

int main(void) {
    char *hello = "Hello, libc-free world!\n";
    write_linux(STDOUT, hello, Size_chars(hello));
    exit_linux(42);
}
