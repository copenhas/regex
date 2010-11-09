#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "engine.h"
#include "boolean.h"

void translate(char *, Regex);
RegexElement create_element();
int matchhere(Regex, int , char *);

bool RE_CHAR_match(RegexElement re, char *text, int *loc);
bool RE_STAR_match(RegexElement re, char *text, int *loc);
bool RE_CLASS_match(RegexElement re, char *text, int *loc);
bool RE_START_ANCHOR_match(RegexElement re, char *text, int *loc);
bool RE_END_ANCHOR_match(RegexElement re, char *text, int *loc);
bool RE_ALT_match(RegexElement re, char *text, int *loc);


Regex REGEX_new(char *regexp)
{
    Regex regex = (Regex)malloc(sizeof(struct REGEX));
    regex->pattern = regexp;
    translate(regexp, regex);
    return regex;
}

void REGEX_free(Regex regex)
{
    int i;
    for(i = 0; i < regex->len; i++)
    {
        if (regex->exp[i]->child != NULL)
        {
            free(regex->exp[i]->child);
            regex->exp[i]->child = NULL;
        }

        free(regex->exp[i]);
        regex->exp[i] = NULL;
    }

    free(regex->exp);
    regex->exp = NULL;
    
    free(regex);
    regex = NULL;
}

bool REGEX_match(Regex regex, char *text)
{
    int loc = 0;
    do {
        if (matchhere(regex, loc, text))
            return MATCH;
    } while (text[loc++] != '\0');

    return NO_MATCH;
}

void translate(char *regexp, Regex regex)
{
    //need to just use the int * passed in and make sure it isn't NULL
    int exp_len = 0;
    struct RE **exp = NULL;

    bool in_cls = FALSE;
    bool start_cls = FALSE;
    struct RE *element;

    int i;
    int relen = strlen(regexp);
    for(i = 0; i < relen; i++)
    {
        switch (regexp[i]){
            case '*':
                element = create_element(RE_STAR);
                element->child = exp[--exp_len];
                break;
            case '[':
                in_cls = TRUE;
                start_cls = TRUE;
                element = create_element(RE_CLASS);
                break;
            case ']':
                in_cls = FALSE;
                break;
            case '$':
                if (i == (relen -1))
                {
                    element = create_element(RE_END_ANCHOR);
                    break;
                }
                /* continue down to the default */
            case '^':
                if (i == 0)
                {
                    element = create_element(RE_START_ANCHOR);
                    break;
                }
                /* else then just continue down to the default case */
            default:
                if (in_cls == FALSE)
                {
                    element = create_element(RE_CHAR);
                    element->ch = regexp[i];
                }
                else
                {
                    if (start_cls == TRUE && regexp[i] == '^')
                    {
                            element->nccl = TRUE;
                    }
                    else 
                    {
                        if (start_cls == TRUE)
                            start_cls = FALSE;

                        if (element->ccl == NULL)
                        {
                            element->ccl = (char *)malloc(sizeof(char) * 11);
                        }

                        int length = strlen(element->ccl);
                        if(length % 10 == 0)
                        {
                            int newsize = length + 11;
                            char *newstr = (char *)malloc(sizeof(char) * newsize);
                            element->ccl = strncpy(newstr, element->ccl, length);
                        }

                        strncat(element->ccl, &regexp[i], 1);
                    }
                }
        }

        if (in_cls == FALSE)
        {
            exp_len++;
            exp = (struct RE **)realloc(exp, sizeof(struct RE *) * exp_len);
            exp[exp_len - 1] = element;
            element = NULL;
        }
    }

    regex->len = exp_len;
    regex->exp = exp;
}

RegexElement create_element(int type)
{
    RegexElement element = (RegexElement)malloc(sizeof(struct RE));
    element->type = type;
    element->child = NULL;
    element->ch = '\0';
    element->ccl = NULL;
    element->nccl = FALSE;

    return element;
}

bool RE_CHAR_match(RegexElement re, char *text, int *loc)
{
    if (re->ch == text[*loc])
    {
        *loc = *loc + 1;
        return MATCH;
    }
    else
    {
        return NO_MATCH;
    }
}

bool RE_STAR_match(RegexElement re, char *text, int *loc)
{
    /* Need to consume any text that matches */
    bool match = NO_MATCH;
    do {
        match = re_match(re->child, text, loc);
    } while (text[*loc] != '\0' && match == MATCH);

    /* Star can match nothing or something so always true */
    return TRUE;
}

bool RE_CLASS_match(RegexElement re, char *text, int *loc)
{
    int i;
    int cls_len = strlen(re->ccl);
    for (i = 0; i < cls_len; i++)
    {
        if (re->nccl == FALSE && re->ccl[i] == text[*loc])
        {
            *loc = *loc + 1;
            return MATCH;
        }
        else if (re->nccl == TRUE && re->ccl[i] == text[*loc])
        {
            return NO_MATCH;
        }
        else if (re->nccl == TRUE && i == (cls_len - 1) 
                && re->ccl[i] != text[*loc])
        {
            *loc = *loc + 1;
            return MATCH;
        }
    }

    return NO_MATCH;
}

bool RE_START_ANCHOR_match(RegexElement re, char *text, int *loc)
{
    if (*loc == 0)
        return TRUE; 
    else
        return FALSE;
}

bool RE_END_ANCHOR_match(RegexElement re, char *text, int *loc)
{
    if (text[*loc] == '\0')
        return TRUE;
    else
        return FALSE;
}

int matchhere(Regex regex, int loc, char *text)
{
    RegexElement *exp = regex->exp;

    int i;
    for (i = 0; i < regex->len; i++)
    {
        if (re_match(exp[i], text, &loc) == FALSE)
            return NO_MATCH;
    }

    return MATCH;
}

int re_match(RegexElement re, char *text, int *loc)
{
    switch (re->type){
        case RE_CHAR:
            return RE_CHAR_match(re, text, loc);
        case RE_STAR:
            return RE_STAR_match(re, text, loc);
        case RE_CLASS:
            return RE_CLASS_match(re, text, loc);
        case RE_START_ANCHOR:
            return RE_START_ANCHOR_match(re, text, loc);
        case RE_END_ANCHOR:
            return RE_END_ANCHOR_match(re, text, loc);
        default:
            return FALSE;
    }
}
