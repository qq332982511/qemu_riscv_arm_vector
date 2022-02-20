#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <arm_sve.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

volatile unsigned int *const UART0DR = (unsigned int *)0x09000000;

void print_uart0(const char *s) {
  while (*s != '\0') {             /* Loop until end of string */
    *UART0DR = (unsigned int)(*s); /* Transmit char */
    s++;                           /* Next char */
  }
}
// syscall stubs
int _close(int fd) {
  errno = EBADF;
  return -1;
}

int _isatty(int fd) { return 1; }

int _fstat(int fd, struct stat *st) {
  errno = EBADF;
  return -1;
}

off_t _lseek(int fd, off_t ptr, int dir) {
  errno = EBADF;
  return (off_t)-1;
}

int _read(int fd, void *ptr, size_t len) {
  errno = EBADF;
  return -1;
}
int _write(int fd, const void *buf, size_t count) {
  const char *c_buf = buf;
  for (size_t i = 0; i < count; ++i) {
    *UART0DR = (unsigned int)(*c_buf); /* Transmit char */
    c_buf++;                           /* Next char */
  }
  return count;
}

static uint64_t semihosting_call(uint32_t operation, uint64_t parameter) { __asm("HLT #0xF000"); }
extern void run_test();
void main() {
  print_uart0("Hello world!\n");
  run_test();
  print_uart0("Bye world!\n");
  //! shutdown system by host call, delete it when real board
  semihosting_call(0x18, 0x20026);
}
