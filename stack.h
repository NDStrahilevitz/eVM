#ifndef STACK_H_
#define STACK_H_

#include "exitcodes.h"
#include <stdint.h>

//a fixed sized stack of 32 bit numbers
typedef struct stack{
    int size; //size of stack
    int top; //index of top value
    uint32_t* arr; //array where stack is stored
} Stack;

Stack* stack_new(int cap);
void stack_free(Stack *s);
int stack_is_empty(Stack* s);
int stack_is_full(Stack* s);
uint32_t stack_peek(Stack* s);
void stack_push(Stack* s, uint32_t val);
uint32_t stack_pop(Stack* s);

#endif


