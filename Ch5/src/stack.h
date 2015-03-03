#ifndef STACK_H
#define STACK_H

#define MAXVAL 100	/* maximum depth of val stack */
#define STACK_EMPTY -1

struct Stack {
  int sp = 0;			/* next free stack position */
  double val[MAXVAL];	/* value stack */
  char kind[MAXVAL];	/* kinds for value stack */
};

// NOTE(brendan) push: push f onto value stack
void push(Stack *s, double f, char k);

// NOTE(brendan): pop: pop and return top value from stack
double pop(Stack *s);

// NOTE(brendan): print top element of stack */
int peek(Stack *s);

// NOTE(brendan): swap top two elements of stack
int swap(Stack *s);

// NOTE(brendan):  duplicate top element of stack
int duplicate(Stack *s);

// NOTE(brendan): clear the stack
int clear(Stack *s);

#endif /* STACK_H */
