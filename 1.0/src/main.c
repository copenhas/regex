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

    Regex regex = REGEX_new(pattern); 
    bool is_match = REGEX_match(regex, text);
    REGEX_free(&regex);

    if (is_match == true)
        printf("True\n");
    else
        printf("False\n");

    return 0;
}
