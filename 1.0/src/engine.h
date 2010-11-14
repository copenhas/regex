#ifndef __ENGINE_H__
#define __ENGINE_H__


#include "boolean.h"

/* types of regular expression elements supported */
enum { RE_CHAR, RE_STAR, RE_CLASS, RE_START_ANCHOR, RE_END_ANCHOR };

typedef struct RE {
    int type;
    struct RE *child;
    int ch;
    char *ccl;
    bool nccl;
} *RegexElement;

typedef struct REGEX {
    int len;
    RegexElement *exp;
    char *pattern;
} *Regex;

Regex REGEX_new(char *pattern);
void REGEX_free(Regex *);
bool REGEX_match(Regex, char *);

#endif
