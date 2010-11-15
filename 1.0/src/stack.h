
#ifndef __STACK_H__
#define __STACK_H__

typedef struct stack {
    int length;
    int size;
    void **elements;
} *Stack;

Stack stack_new();

void stack_free(Stack *);

void *stack_pop(Stack);

void stack_push(Stack, void *);

#endif
