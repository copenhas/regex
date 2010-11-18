#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "engine.h"
#include "stack.h"

RegexElement RE_new();
void RE_free(RegexElement *);
void translate(char *, Regex);
void RE_CLASS_add_char(RegexElement, char);
bool matchhere(Regex, int , char *);

bool RE_match(RegexElement, Stack, EngineState); 
bool RE_CHAR_match(RegexElement, Stack, EngineState); 
bool RE_STAR_match(RegexElement, Stack, EngineState); 
bool RE_CLASS_match(RegexElement, Stack, EngineState); 
bool RE_START_ANCHOR_match(RegexElement, Stack, EngineState); 
bool RE_END_ANCHOR_match(RegexElement, Stack, EngineState); 
bool RE_ALT_match(RegexElement, Stack, EngineState); 


Regex REGEX_new(char *regexp)
{
    Regex regex = (Regex)malloc(sizeof(struct REGEX));
    if (regex == NULL) return NULL;

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

bool REGEX_match(Regex regex, char *text)
{
    int loc = 0;
    do {
        if (matchhere(regex, loc, text)) return true;
    } while (text[loc++] != '\0');

    return false;
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
                element = RE_new(RE_STAR);
                element->child = exp[--exp_len];
                break;
            case '[':
                in_cls = true;
                start_cls = true;
                element = RE_new(RE_CLASS);
                break;
            case ']':
                in_cls = false;
                break;
            case '$':
                if (i == (relen -1))
                {
                    element = RE_new(RE_END_ANCHOR);
                    break;
                }
                /* continue down to the default */
            case '^':
                if (i == 0)
                {
                    element = RE_new(RE_START_ANCHOR);
                    break;
                }
                /* else then just continue down to the default case */
            default:
                if (in_cls == false)
                {
                    element = RE_new(RE_CHAR);
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

RegexElement RE_new(int type)
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
    EngineState initial = state_new(regex->exp, 0, text, loc);
    stack_push(stack, initial);


    bool match = false;

    do
    {
        EngineState current = stack_pop(stack);

        match = false;
        RegexElement *exp = regex->exp;
        int first_element = current->exp_index;
        int last_element = regex->len;
        int i;
        for (i = first_element; i < last_element; i++)
        {
            match = RE_match(exp[i], stack, current);
            if (match == false) break;
        }

        state_free(&current);

    } while(stack->length > 0 && match == false);

    stack_free(&stack);

    return match;
}

bool RE_match(RegexElement re, Stack stack, EngineState current) 
{
    switch (re->type){
        case RE_CHAR:
            return RE_CHAR_match(re, stack, current); 
        case RE_STAR:
            return RE_STAR_match(re, stack, current); 
        case RE_CLASS:
            return RE_CLASS_match(re, stack, current); 
        case RE_START_ANCHOR:
            return RE_START_ANCHOR_match(re, stack, current); 
        case RE_END_ANCHOR:
            return RE_END_ANCHOR_match(re, stack, current); 
        default:
            return false;
    }
}

bool RE_CHAR_match(RegexElement re, Stack stack, EngineState current) 
{
    if (re->ch == current->text[current->text_index])
    {
        current->text_index++;
        return true;
    }
    else
    {
        return false;
    }
}

bool RE_STAR_match(RegexElement re, Stack stack, EngineState current) 
{
    /* Need to consume any text that matches */
    bool match = false;
    do {
        match = RE_match(re->child, stack, current); 

        if(match == true)
        {
            /* 
             * since a * eating too much text could cause the
             * expression to fail as a whole we need to record
             * states to try when backtracking
             */
            //EngineState rewind = state_clone(current);
            EngineState rewind = state_clone(*current);
            rewind->exp_index++;
            stack_push(stack, rewind);
        }
    } while (current->text[current->text_index] != '\0' && match == true);

    /* Star can match nothing or something so always true */
    return true;
}

bool RE_CLASS_match(RegexElement re, Stack stack, EngineState current) 
{
    int loc = current->text_index;
    int i;
    int cls_len = strlen(re->ccl);
    for (i = 0; i < cls_len; i++)
    {
        if (re->nccl == false && re->ccl[i] == current->text[loc])
        {
            current->text_index++;
            return true;
        }
        else if (re->nccl == true && re->ccl[i] == current->text[loc])
        {
            return false;
        }
        else if (re->nccl == true && i == (cls_len - 1) 
                && re->ccl[i] != current->text[loc])
        {
            current->text_index++;
            return true;
        }
    }

    return false;
}

bool RE_START_ANCHOR_match(RegexElement re, Stack stack, EngineState current) 
{
    if (current->text_index == 0)
        return true; 
    else
        return false;
}

bool RE_END_ANCHOR_match(RegexElement re, Stack stack, EngineState current) 
{
    if (current->text[current->text_index] == '\0')
        return true;
    else
        return false;
}

