
#ifndef __STACK_H__
#define __STACK_H__

#include "engine.h"

typedef struct state {
    RegexElement *elements;
    int exp_index;
    char *text;
    int text_index;
} *EngineState;

EngineState state_new(RegexElement *, int, char *, int);
EngineState state_clone(const struct state);
void state_free(EngineState *);


typedef struct stack {
    int length;
    int size;
    EngineState *elements;
} *Stack;

Stack stack_new();
void stack_free(Stack *);
EngineState stack_pop(Stack);
void stack_push(Stack, EngineState);

#endif
