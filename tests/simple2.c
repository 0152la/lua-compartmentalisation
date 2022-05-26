#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

int __real___vdso_gettimeofday(struct timeval *tv, struct timezone *tz);
int __wrap___vdso_gettimeofday(struct timeval *tv, struct timezone *tz) {
  puts("In __wrap___vdso_gettimeofday(...)");
  return __real___vdso_gettimeofday(tv, tz);
}

int main(int argc, char *argv[]) {
  struct timeval tv;
  printf("Calling gettimeofday(...)...\n");
  int result = gettimeofday(&tv, NULL);
  if (result == 0) {
    printf("Success! tv_sec = %lu\n", tv.tv_sec);
  } else {
    printf("Error (%d): %s\n", result, strerror(errno));
  }
}
