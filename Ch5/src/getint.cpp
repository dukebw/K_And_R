/* ========================================================================
   File: getint.cpp
   Date: Feb. 10/15
   Revision: 1
   Creator: Brendan Duke
   Notice: (C) Copyright 2015 by BRD Inc. All Rights Reserved.
   ======================================================================== */

#include <ctype.h>
#include <stdio.h>

int getch(void);
void ungetch(int);

// NOTE(brendan): returns next non-whitespace character
char skipWhiteSpace() {
  char c;
  // NOTE(brendan): skip white-space
  while(isspace(c = getch()));
  return c;
}

// NOTE(brendan): getint: get next integer from input into *pn
int getint(int *pn) {
  int c, sign;

  c = skipWhiteSpace();

  if(!isdigit(c) && c != EOF && c != '+' && c != '-') {
    // NOTE(brendan): it's not a number
    ungetch(c);
    return 0;
  }

  sign = (c == '-') ? -1 : 1;
  if(c == '+' || c == '-') {
    c = getch();
    c = skipWhiteSpace();
  }
  for(*pn = 0; isdigit(c); c = getch()) {
    *pn = 10 * *pn + (c - '0');
  }
  *pn *= sign;
  if(c != EOF) {
    ungetch(c);
  }
  return c;
}

// NOTE(brendan): getfloat: get next float from input into *pn
int getfloat(float *outFloat) {
  int c, sign;

  c = skipWhiteSpace();

  if(!isdigit(c) && c != EOF && c != '+' && c != '-') {
    // NOTE(brendan): it's not a number
    ungetch(c);
    return 0;
  }

  sign = (c == '-') ? -1 : 1;
  if(c == '+' || c == '-') {
    c = getch();
    c = skipWhiteSpace();
  }
  for(*outFloat = 0; isdigit(c); c = getch()) {
    *outFloat = 10.0 * *outFloat + (c - '0');
  }
  if(c == '.') {
    c = getch();
    float power;
    for(power = 1.0; isdigit(c); c = getch()) {
      *outFloat = 10.0 * *outFloat + (c - '0');
      power *= 10.0;
    }
    *outFloat = *outFloat / power;
  }
  *outFloat *= sign;
  if(c != EOF) {
    ungetch(c);
  }
  return c;
}

#define BUFSIZE 100

char buf[BUFSIZE];	/* buffer for ungetch */
int bufp = 0;		/* next free position in buf */

// NOTE(brendan): get a (possibly pushed back) character
int getch(void)	{
	return (bufp > 0) ? buf[--bufp] : getchar();
}

// NOTE(brendan): push character back on input
void ungetch(int c)	{
	if (bufp >= BUFSIZE) {
		printf("ungetch: too many characters\n");
  }
	else {
		buf[bufp++] = c;
  }
}

int main(void) {
  float x;
  while(getfloat(&x)) {
    printf("%f\n", x);
  }
}
