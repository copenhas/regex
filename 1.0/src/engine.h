#ifndef __ENGINE_H__
#define __ENGINE_H__

#define MATCH 1
#define NO_MATCH 0

#ifndef __BOOLEAN_H__
#include "boolean.h"
#endif

/* types of regular expression elements supported */
#define RE_CHAR 0
#define RE_STAR 1
#define RE_CLASS 2
#define RE_START_ANCHOR 3
#define RE_END_ANCHOR 4

typedef struct RE {
    int type;
    struct RE *child;
    int ch;
    char *ccl;
    bool nccl;
} RE;

typedef struct REGEX {
    int len;
    struct RE ** exp;
    char *pattern;
} REGEX;

struct REGEX *REGEX_new(char *pattern);
void REGEX_free(struct REGEX *);
bool REGEX_match(struct REGEX*, char *);

#endif
