#include <stdlib.h>
#include <stdio.h>
#include "stack.h"


Stack stack_new()
{
    return (Stack)malloc(sizeof(struct stack));
}

void stack_free(Stack *stack)
{
    free(*stack);
    *stack = NULL;
}

void *stack_pop(Stack stack)
{
    if(stack == NULL) return NULL;
    if(stack->last_index == 0) return NULL;

    return stack->elements[stack->last_index--];
}

void stack_push(Stack stack, void *element)
{

}
