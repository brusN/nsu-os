#include "mylib.h"

// Only for decimal numbers
int str2long(long *number, char *start) {
    // Is null or empty string check
    if (start == NULL || start[0] == '\0') {
        return str2num_EMPTY_STR;
    }

    // Is string a number check
    {
        if (!(start[0] == '+' || start[0] == '-' || isdigit(start[0]))) {
            return str2num_NOT_NUMBER;
        }
        int i = 1;
        while (isdigit(start[i])) {
            ++i;
        }
        if (start[i] != '\0') {
            return str2num_NOT_NUMBER;
        }
    }

    *number = strtol(start, NULL, 10);
    if (errno == ERANGE) {
        return str2num_ERANGE;
    }
    return str2num_SUCCESS;
}


