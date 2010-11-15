#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "engine.h"
#include "stack.h"

void translate(char *, Regex);
RegexElement create_element();
bool matchhere(Regex, int , char *);

bool RE_match(RegexElement re, char *text, int *loc, Stack *stack);
bool RE_CHAR_match(RegexElement re, char *text, int *loc, Stack *stack);
bool RE_STAR_match(RegexElement re, char *text, int *loc, Stack *stack);
bool RE_CLASS_match(RegexElement re, char *text, int *loc, Stack *stack);
bool RE_START_ANCHOR_match(RegexElement re, char *text, int *loc, Stack *stack);
bool RE_END_ANCHOR_match(RegexElement re, char *text, int *loc, Stack *stack);
bool RE_ALT_match(RegexElement re, char *text, int *loc, Stack *stack);


Regex REGEX_new(char *regexp)
{
    Regex regex = (Regex)malloc(sizeof(struct REGEX));
    regex->pattern = regexp;
    translate(regexp, regex);
    return regex;
}

void REGEX_free(Regex *regex_ptr)
{
    Regex regex = *regex_ptr; 
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
    *regex_ptr = NULL;
}

bool REGEX_match(Regex regex, char *text)
{
    int loc = 0;
    do {
        if (matchhere(regex, loc, text)) return true;
    } while (text[loc++] != '\0');

    return false;
}

void translate(char *regexp, Regex regex)
{
    //need to just use the int * passed in and make sure it isn't NULL
    int exp_len = 0;
    struct RE **exp = NULL;

    bool in_cls = false;
    bool start_cls = false;
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
                in_cls = true;
                start_cls = true;
                element = create_element(RE_CLASS);
                break;
            case ']':
                in_cls = false;
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
                if (in_cls == false)
                {
                    element = create_element(RE_CHAR);
                    element->ch = regexp[i];
                }
                else
                {
                    if (start_cls == true && regexp[i] == '^')
                    {
                            element->nccl = true;
                    }
                    else 
                    {
                        if (start_cls == true)
                            start_cls = false;

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

        if (in_cls == false)
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
    element->nccl = false;

    return element;
}

bool matchhere(Regex regex, int loc, char *text)
{
    Stack stack = stack_new();

    RegexElement *exp = regex->exp;

    bool match = true;
    int i;
    for (i = 0; i < regex->len; i++)
    {
        if (RE_match(exp[i], text, &loc, &stack) == false)
        {
            match = false;
            break;
        }
    }

    stack_free(&stack);

    return match;
}

bool RE_match(RegexElement re, char *text, int *loc, Stack *stack)
{
    switch (re->type){
        case RE_CHAR:
            return RE_CHAR_match(re, text, loc, stack);
        case RE_STAR:
            return RE_STAR_match(re, text, loc, stack);
        case RE_CLASS:
            return RE_CLASS_match(re, text, loc, stack);
        case RE_START_ANCHOR:
            return RE_START_ANCHOR_match(re, text, loc, stack);
        case RE_END_ANCHOR:
            return RE_END_ANCHOR_match(re, text, loc, stack);
        default:
            return false;
    }
}

bool RE_CHAR_match(RegexElement re, char *text, int *loc, Stack *stack)
{
    if (re->ch == text[*loc])
    {
        *loc = *loc + 1;
        return true;
    }
    else
    {
        return false;
    }
}

bool RE_STAR_match(RegexElement re, char *text, int *loc, Stack *stack)
{
    /* Need to consume any text that matches */
    bool match = false;
    do {
        match = RE_match(re->child, text, loc, stack);
    } while (text[*loc] != '\0' && match == true);

    /* Star can match nothing or something so always true */
    return true;
}

bool RE_CLASS_match(RegexElement re, char *text, int *loc, Stack *stack)
{
    int i;
    int cls_len = strlen(re->ccl);
    for (i = 0; i < cls_len; i++)
    {
        if (re->nccl == false && re->ccl[i] == text[*loc])
        {
            *loc = *loc + 1;
            return true;
        }
        else if (re->nccl == true && re->ccl[i] == text[*loc])
        {
            return false;
        }
        else if (re->nccl == true && i == (cls_len - 1) 
                && re->ccl[i] != text[*loc])
        {
            *loc = *loc + 1;
            return true;
        }
    }

    return false;
}

bool RE_START_ANCHOR_match(RegexElement re, char *text, int *loc, Stack *stack)
{
    if (*loc == 0)
        return true; 
    else
        return false;
}

bool RE_END_ANCHOR_match(RegexElement re, char *text, int *loc, Stack *stack)
{
    if (text[*loc] == '\0')
        return true;
    else
        return false;
}

