#include <stdio.h>
#include <sys/time.h>

/*int __real___vdso_clock_gettime(clockid_t clock_id, struct timespec* ts);*/
/*int __wrap___vdso_clock_gettime(clockid_t clock_id, struct timespec* ts)*/
/*{*/
    /*return __real___vdso_clock_gettime(clock_id, ts);*/
/*}*/

int
main(void)
{
    time_t seconds = time(NULL);
    return seconds;
}
