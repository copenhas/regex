#include <stdlib.h>
#include <stdio.h>
#include "stack.h"


Stack stack_new()
{
    Stack stack = (Stack)malloc(sizeof(struct stack));

    stack->size = 10;
    stack->length = 0;
    stack->elements = (void **)calloc(stack->size, sizeof(void *));

    return stack;
}

void stack_free(Stack *stack)
{
    free(*stack);
    *stack = NULL;
}

void *stack_pop(Stack stack)
{
    if(stack == NULL) return NULL;
    if(stack->length == 0) return NULL;

    return stack->elements[--stack->length];
}

void stack_push(Stack stack, void *element)
{
    if(stack->length == stack->size)
    {
        stack->size *= 2;
        stack->elements = (void **)realloc(stack->elements, stack->size * sizeof(void *)); 
    }

    stack->elements[stack->length++] = element;
}
