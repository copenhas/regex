
#ifndef __STACK_H__
#define __STACK_H__

#include "engine.h"

typedef struct state {
    int exp_index;
    int text_index;
} *EngineState;

EngineState state_new(int, int);
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
