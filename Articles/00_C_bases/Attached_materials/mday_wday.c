#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>

int
main(int argc, char** argv)
{
    if (argc < 4) {
        return 1;
    }

    char* eptr = NULL;
    errno = 0;
    long year = strtol(argv[1], &eptr, 10);
    if (errno || *eptr || eptr == argv[1] || (int) year != year) {
        return 1;
    }
    
    eptr = NULL;
    errno = 0;
    long day = strtol(argv[2], &eptr, 10);
    if (errno || *eptr || eptr == argv[2] || (int) day != day) {
        return 1;
    }
	
	eptr = NULL;
    errno = 0;
    long wday = strtol(argv[3], &eptr, 10);
    if (errno || *eptr || eptr == argv[4] || (int) wday != wday) {
        return 1;
    }

    int count = 0;

    for (int i = 0; i < 12; i++) {
        struct tm time;

        time.tm_sec = 0;
        time.tm_min = 0;
        time.tm_hour = 0;
        time.tm_mday = day;
        time.tm_mon = i;    
        time.tm_year = year - 1900;
        time.tm_wday = 0;
        time.tm_yday = 0;
        time.tm_isdst = -1;
        mktime(&time);

        if (time.tm_wday == wday && time.tm_mon == i) {
            count++;
        }
    }

    printf("%d\n", count);

    return 0;
}
