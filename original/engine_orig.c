/*
 * Code copied from Beautiful Code "Chatper 1: A Regular Expression Matcher"
 * in order to play around and possibly expand.
 */
#include "engine.h"
#include "boolean.h"

/* match: search for regexp anywhere in text */
int match(char *regexp, char *text)
{
    if (regexp[0] == '^')
        return matchhere(regexp+1, text);
    do {
        if (matchhere(regexp, text))
            return MATCH;
    } while (*text++ != '\0');
    return NO_MATCH;
}

/* matchhere: search for regexp at beginning of text */
int matchhere(char *regexp, char *text)
{
    if (regexp[0] == '\0')
        return MATCH;
    if (regexp[1] == '*')
        return matchstar(regexp[0], regexp+2, text);
    if (regexp[0] == '$' && regexp[1] == '\0')
        return *text == '\0';
    if (*text!='\0' && (regexp[0] == '.' || regexp[0] == *text))
        return matchhere(regexp+1, text+1);
    return NO_MATCH;
}

/*matchstar: search for c*regexp at beginning of text */
int matchstar(int c, char *regexp, char *text)
{
    do { /* a * matches zero or more instances */
        if (matchhere(regexp, text))
            return MATCH;
    } while (*text != '\0' && (*text++ == c || c == '.'));
    return NO_MATCH;
}
