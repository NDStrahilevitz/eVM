#include "stack.h"
#include  <stdlib.h>

Stack* stack_new(int cap){
    Stack* s = (Stack*)malloc(sizeof(sizeof(Stack)));

    s->size = cap;
    s->top = -1;
    s->arr = (uint16_t*)malloc(cap*sizeof(uint16_t));

    return s;
}

int stack_is_empty(Stack* s){
    return s->top == -1;
}

int stack_is_full(Stack* s){
    return s->top == s->size - 1;
}

uint16_t stack_peek(Stack* s){
    if(stack_is_empty(s)){
        exit(MEMORY_OUT_OF_BOUND);
    }
    return s->arr[s->top];
}

void stack_push(Stack* s, uint16_t val){
    if(stack_is_full(s)){
        exit(OVERFLOW_ERROR);
    }
    s->arr[++s->top] = val;
}

uint16_t stack_pop(Stack* s){
    if(stack_is_empty(s)){
        exit(OVERFLOW_ERROR);
    }
    return s->arr[s->top--];
}



