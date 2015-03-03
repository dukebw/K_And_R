/* ========================================================================
File: expr.cpp
Date: Mar. 2/2015
Revision: 1
Creator: Brendan Duke
Notice: (C) Copyright 2015 by BRD Inc. All Rights Reserved.
Evaluates a reverse Polish expression from the command line.
E.g. expr 2 3 4 + *
Evaluates to 2x(3+4)
======================================================================== */

#include "util.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>	/* for atof() */
#include <math.h>	/* for sin, exp and pow */
#include <ctype.h>

#define MAXOP 100	/* max size of operand or operator */
#define NUMBER '0'	/* signal that a number was found */
#define PRINT ';'
#define SINE '~'
#define POWER '^'
#define EXPO 'E'
#define ASSIGN '='
#define LETTERS 25
#define RECENT 'Z'

double vars[LETTERS];	/* variables' values */

// NOTE(brendan): assigns a value to a variable name
int assign(Stack *s) {
  int err_chk = 0;

  if((s->sp > 1) && (s->kind[s->sp - 2] != NUMBER)) {
    for(int i = 0; i < s->sp; ++i) {
      if(s->kind[i] == s->kind[s->sp - 2]) {
        s->val[i] = s->val[s->sp - 1];
      }
    }
    vars[s->kind[s->sp - 2]] = pop(s);
    pop(s);
  }
  else if (s->sp > 1)
    printf("must assign to a variable\n");
  else {
    err_chk = STACK_EMPTY;
  }
  return err_chk;
}

// NOTE(brendan): getop: get next operator or numeric operand
// TODO(brendan): Error checking?
int getop(char s[], char stringArgs[]) {
  int c, i = 0;

  s[0] = c = stringArgs[i++];
  s[1] = '\0';

  if(!isdigit(c) && c != '.' && c != '-' && c != '+') {
    return c;	/* not a number */
  }

  int j = 0;
  if((c == '-' || c == '+') && !isdigit(s[++j] = c = stringArgs[i++])) {
    s[1] = '\0';
    return s[0];
  }
  else {	/* collect integer part */
    while (isdigit(s[++j] = c = stringArgs[i++]));
  }
  if(c == '.') {	/* collect fractional part */
    while(isdigit(s[++j] = c = stringArgs[i++]));
  }
  s[j] = '\0';
  return NUMBER;
}

int main(int argc, char *argv[]) {
  int type = 0, check = 0;
  double op2;
  char s[MAXOP];
  Stack exprStack = {};

  int argIndex = 1;
  while(--argc > 0) {
    type = getop(s, argv[argIndex++]);
    switch (type) {
      case NUMBER:
      {
        push(&exprStack, atof(s), NUMBER);
      } break;
      case '+':
      {
        push(&exprStack, pop(&exprStack) + pop(&exprStack), NUMBER);
      } break;
      case '*':
      {
        push(&exprStack, pop(&exprStack) * pop(&exprStack), NUMBER);
      } break;
      case '-':
      {
        op2 = pop(&exprStack);
        push(&exprStack, pop(&exprStack) - op2, NUMBER);
      } break;
      case '/':
      {
        op2 = pop(&exprStack);
        if (op2 != 0.0)
          push(&exprStack, pop(&exprStack) / op2, NUMBER);
        else
          printf("error: zero divisor\n");
      } break;
      case '%':
      {
        op2 = pop(&exprStack);
        if (op2 != 0.0)
          push(&exprStack, (int) pop(&exprStack) % (int) op2, NUMBER);
        else
          printf("error: zero divisor\n");
      } break;
      case 'P':
      {
        check = peek(&exprStack);
      } break;
      case 'D':
      {
        check = duplicate(&exprStack);
      } break;
      case 'S':
      {
        check = swap(&exprStack);
      } break;
      case 'C':
      {
        check = clear(&exprStack);
      } break;
      case SINE:
      {
        push(&exprStack, sin(pop(&exprStack)), NUMBER);
      } break;
      case POWER:
      {
        op2 = pop(&exprStack);
        push(&exprStack, pow(pop(&exprStack), op2), NUMBER);
      } break;
      case EXPO:
      {
        push(&exprStack, pow(10,pop(&exprStack)), NUMBER);
      } break;
      case ASSIGN:
      {
        check = assign(&exprStack);
      } break;
      case PRINT:
      {
        printf("\t%.8g\n", pop(&exprStack));
      } break;
      default:
      {
        if(islower(type)) {
          push(&exprStack, vars[type - 'a'], type - 'a');
        }
        else {
          printf("error: unknown command %s\n", s);
        }
      } break;
    }
    if (check == STACK_EMPTY) {
      printf("error: not enough elements in stack\n");
      check = 0;
    }
  }
  return 0;
}
