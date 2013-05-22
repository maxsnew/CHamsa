#ifndef _TOKENEX_STACK_H
#define _TOKENEX_STACK_H
//quick code copied from howstuff works..
//http://computer.howstuffworks.com/c33.htm

#include "interval.h"

/* Stack Library - This library offers the
   minimal stack operations for a
   stack of integers (easily changeable) */

typedef interval stack_data;

extern void stack_init();
/* Initializes this library.
   Call first before calling anything. */

extern void stack_clear();
/* Clears the stack of all entries. */

extern int stack_empty();
/* Returns 1 if the stack is empty, 0 otherwise. */

extern void stack_push(stack_data d);
/* Pushes the value d onto the stack. */

extern stack_data stack_pop();
/* Returns the top element of the stack,
   and removes that element.
   Returns garbage if the stack is empty. */


//added by cowboy.
extern stack_data stack_top();

#endif
