#include <stdlib.h>
#include <stdio.h>
#include "stack.h"


Stack stack_new()
{
    Stack stack = (Stack)malloc(sizeof(struct stack));

    stack->size = 10;
    stack->length = 0;
    stack->elements = (EngineState *)calloc(stack->size, sizeof(EngineState *));

    return stack;
}

void stack_free(Stack *stack)
{
    free((*stack)->elements);
    free(*stack);
    *stack = NULL;
}

EngineState stack_pop(Stack stack)
{
    if(stack == NULL) return NULL;
    if(stack->length == 0) return NULL;

    return stack->elements[--stack->length];
}

void stack_push(Stack stack, EngineState element)
{
    if(stack->length == stack->size)
    {
        stack->size *= 2;
        stack->elements = (EngineState *)realloc(stack->elements, 
                                           stack->size * sizeof(EngineState *)); 
    }

    stack->elements[stack->length++] = element;
}

EngineState state_new(int exp_index, int text_index)
{
    EngineState state = (EngineState)malloc(sizeof(state));
    state->exp_index = exp_index;
    state->text_index = text_index;

    return state;
}

void state_free(EngineState *state)
{
   free(*state); 
   *state = NULL;
}
