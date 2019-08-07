#include "exitcodes.h"
#include <stdint.h>

#ifndef STACK_H_
#define STACK_H_

//a fixed sized stack of 16 bit numbers
typedef struct stack{
    int size; //size of stack
    int top; //index of top value
    uint16_t* arr; //array where stack is stored
} Stack;

Stack* stack_new(int cap);
int stack_is_empty(Stack* s);
int stack_is_full(Stack* s);
uint16_t stack_peek(Stack* s);
void stack_push(Stack* s, uint16_t val);
uint16_t stack_pop(Stack* s);

#endif


