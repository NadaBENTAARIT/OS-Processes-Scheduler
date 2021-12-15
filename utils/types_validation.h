#ifndef TYPES_VALIDATION_HEADER_FILE
#define TYPES_VALIDATION_HEADER_FILE

#include <string.h>
#include <ctype.h>

int digit_check(char *string)

{

    for (int i = 0; i < strlen(string); i++)

    {

        if (isdigit(*(string+i)) == 0)

        {

            return 0;
        }
    }

    return 1;
}

#endif