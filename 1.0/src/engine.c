#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "engine.h"
#include "stack.h"

struct RE_State 
{
    Regex regex;
    int re_index;
    char *text;
    int *loc;
};

void RE_free(RegexElement *);
void translate(char *, Regex);
RegexElement create_element();
void RE_CLASS_add_char(RegexElement, char);
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
        RE_free(&(regex->exp[i]));
    }

    free(regex->exp);
    regex->exp = NULL;
    
    free(regex);
    *regex_ptr = NULL;
}

void RE_free(RegexElement *element)
{
    RegexElement tofree = *element;
    if (tofree->child != NULL)
    {
        RE_free(&(tofree->child));
        tofree->child = NULL;
    }

    if (tofree->ccl != NULL)
    {
        free(tofree->ccl);
        tofree->ccl = NULL;
    }

    free(tofree);
    *element = NULL; 
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
    RegexElement *exp = NULL;

    bool in_cls = false;
    bool start_cls = false;
    RegexElement element = NULL;

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
                else if (start_cls == true && regexp[i] == '^')
                {
                    element->nccl = true;
                }
                else 
                {
                    if (start_cls == true) start_cls = false;
                    RE_CLASS_add_char(element, regexp[i]);
                }
        }

        if (in_cls == false)
        {
            exp_len++;
            exp = (RegexElement *)realloc(exp, sizeof(struct RE *) * exp_len);
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

void RE_CLASS_add_char(RegexElement element, char ch)
{
    if (element->ccl == NULL)
    {
        element->ccl = (char *)calloc(11, sizeof(char));
    }

    int length = strlen(element->ccl);
    if(length % 10 == 0)
    {
        int newsize = length + 11;
        // this could cause a memory leak for i'm ignoring it
        element->ccl = (char *)realloc(element->ccl, 
                                       sizeof(char) * newsize);
    }

    strncat(element->ccl, &ch, 1);
}

bool matchhere(Regex regex, int loc, char *text)
{
    /*
     * need to initialize the regex stack with the first state to try
     * each of the regex elements get a chance to add to the Stack
     * we should keep trying to match so long as there is a possibly
     * matching state on the stack
     */
    Stack stack = stack_new();
    EngineState initial = state_new(0, loc);
    stack_push(stack, initial);

    bool match = true;

    while(stack->length > 0)
    {
        EngineState current = stack_pop(stack);

        RegexElement *exp = regex->exp;
        int first_element = current->exp_index;
        int last_element = regex->len;
        int text_index = current->text_index;
        int i;
        for (i = first_element; i < last_element; i++)
        {
            if (RE_match(exp[i], text, &text_index, &stack) == false)
            {
                match = false;
                break;
            }
        }

        state_free(&current);
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

