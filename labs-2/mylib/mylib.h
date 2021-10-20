#ifndef NSU_OS_LABS_MYLIB_H
#define NSU_OS_LABS_MYLIB_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>

// For family of functions str2...(4)
enum str2num_states {
    str2num_SUCCESS = 0,
    str2num_ERANGE,
    str2num_NOT_NUMBER,
    str2num_EMPTY_STR
};

// Improved strtol with error handling
int str2long(long *number, char *start, char **end, int radix);

#endif //NSU_OS_LABS_MYLIB_H
