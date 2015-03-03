/* ========================================================================
   File: stack.cpp
   Date: Mar. 2/2015
   Revision: 1
   Creator: Brendan Duke
   Notice: (C) Copyright 2015 by BRD Inc. All Rights Reserved.
   ======================================================================== */

#include "stack.h"
#include <stdio.h>

// NOTE(brendan) push: push f onto value stack
void push(Stack *s, double f, char k) {
	if (s->sp < MAXVAL) {
		s->val[s->sp] = f;
		s->kind[s->sp++] = k;
	}
	else {
		printf("error: stack full, can't push %g\n", f);
  }
}

// NOTE(brendan): pop: pop and return top value from stack
double pop(Stack *s) {
	if (s->sp > 0) {
		return s->val[--s->sp];
  }
	else {
		printf("error: stack empty\n");
		return 0.0;
	}
}

// NOTE(brendan): print top element of stack */
int peek(Stack *s) {
	int err_chk = 0;

	if(s->sp > 0) {
		printf("Top: %g\n", s->val[s->sp - 1]);
  }
	else {
		err_chk = STACK_EMPTY;
  }
	return err_chk;
}

// NOTE(brendan): swap top two elements of stack
int swap(Stack *s) {
	int temp, err_chk;
	char temp_kind = ' ';
	temp = err_chk = 0;

	if(s->sp > 1) {
		temp = s->val[s->sp - 1];
		s->val[s->sp - 1] = s->val[s->sp - 2];
		s->val[s->sp - 2] = temp;
		temp_kind = s->kind[s->sp - 1];
		s->kind[s->sp - 1] = s->kind[s->sp - 2];
		s->kind[s->sp - 2] = temp_kind;
	}
	else{
		err_chk = STACK_EMPTY;
  }
	return err_chk;
}

// NOTE(brendan):  duplicate top element of stack
int duplicate(Stack *s) {
	int err_chk = 0;

	if(s->sp > 0) {
		s->val[s->sp] = s->val[s->sp - 1];
		++s->sp;
	}
	else{
		err_chk = STACK_EMPTY;
  }
	return err_chk;
}

// NOTE(brendan): clear the stack
int clear(Stack *s) {
	int err_chk = 0;

	if(s->sp > 0) {
    s->sp = 0;
  }
	else {
    err_chk = STACK_EMPTY;
  }
	return err_chk;
}
