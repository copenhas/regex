#include <stdio.h>
#include <stdlib.h>
#include "engine.h"

int main(int argc, char *args[])
{
    if (argc != 3)
    {
        printf("Usage: regex <input> <regex>\n");
        return 0;
    }

    char *text = args[1];
    char *pattern = args[2];

    int exp_len;
    struct REGEX *regex = REGEX_new(pattern); 
    REGEX_init(regex, pattern);
    int is_match = REGEX_match(regex, text);
    printf("Match %d\n", is_match);
    REGEX_free(regex);

    return 0;
}
