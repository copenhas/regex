#include <stdlib.h>
#include <stdio.h>
#include "stack.h"


Stack stack_new()
{
    Stack stack = (Stack)malloc(sizeof(struct stack));

    stack->size = 10;
    stack->length = 0;
    stack->elements = (EngineState *)calloc(stack->size, sizeof(EngineState));

    return stack;
}

void stack_free(Stack *stack)
{
    while((*stack)->length > 0)
    {
        EngineState state = stack_pop(*stack);
        state_free(&state);
    }

    free((*stack)->elements);
    free(*stack);
    *stack = NULL;
}

EngineState stack_pop(Stack stack)
{
    if(stack == NULL) return NULL;
    if(stack->length == 0) return NULL;

    EngineState element = stack->elements[stack->length - 1];
    stack->elements[stack->length - 1] = NULL;
    stack->length--;
    return element;
}

void stack_push(Stack stack, EngineState element)
{
    if(stack->length == stack->size)
    {
        stack->size *= 2;
        stack->elements = (EngineState *)realloc(stack->elements, 
                                           stack->size * sizeof(EngineState *)); 
    }

    stack->elements[stack->length] = element;
    stack->length++;
}

EngineState state_new(RegexElement *elements, int exp_index, char *text, int text_index)
{
    EngineState state = (EngineState)malloc(sizeof(struct state));

    state->elements = elements;
    state->exp_index = exp_index;
    state->text = text;
    state->text_index = text_index;

    return state;
}

EngineState state_clone(const struct state to_clone)
{
    return state_new(to_clone.elements, to_clone.exp_index,
                     to_clone.text, to_clone.text_index);
}

void state_free(EngineState *state)
{
   free(*state); 
   *state = NULL;
}
