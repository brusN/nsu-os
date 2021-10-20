#include "mylib.h"

int str2long(long *number, char *start, char **end, int radix) {
    // Is null or empty string check
    if (start == NULL || start[0] == '\0') {
        return str2num_EMPTY_STR;
    }

    // Is string a number check
    {
        int i = 0;
        while (isdigit(start[i])) {
            ++i;
        }
        if (start[i] != '\0') {
            return str2num_NOT_NUMBER;
        }
    }

    *number = strtol(start, end, radix);
    if (errno == ERANGE) {
        return str2num_ERANGE;
    }
    return str2num_SUCCESS;
}


