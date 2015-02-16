/* ========================================================================
   File: ptr_functions.cpp
   Date: Feb. 16/15
   Revision: 1
   Creator: Brendan Duke
   Notice: (C) Copyright 2015 by BRD Inc. All Rights Reserved.
   ======================================================================== */

#include <stdio.h>
#include <ctype.h>

#define MAXLINE 128

// NOTE(brendan): read a line into s; return length
int getline(char s[], int lim) {
	int c;

  char *start = s;
  while((lim-- > 0) && ((c=getchar()) != EOF) && (c != '\n')) {
    *s++ = c;
  }
	if(c == '\n') {
		*s++ = c;
  }
	*s = '\0';
	return (s - start);
}

// NOTE(brendan): convert s to integer
int atoi(char s[]) {
	while(isspace(*s)) {
    ++s;
  }
  int sign;
	sign = (*s == '-') ? -1 : 1;
	if (*s == '+' || *s == '-') {
    ++s;
  }
	int n = 0;
	while(isdigit(*s)) {
		n = 10 * n + (*s - '0');
    ++s;
  }
	return sign * n;
}

// NOTE(brendan): return pointer to t in s; NULL if none
char *strindex(char s[], char t[]) {
  char *sPtr, *tPtr;
	for(; *s != '\0'; s++) {
		for(sPtr = s, tPtr = t; 
        (*tPtr != '\0') && (*sPtr == *tPtr); 
        ++sPtr, ++tPtr);
		if(tPtr > t && *tPtr == '\0') {
			return s;
    }
	}
	return NULL;
}

// NOTE(brendan): testing
int main() {
  char testing0[MAXLINE] = "The quick brown fox jumped over the lazy dog";
  char testing1[] = "dog";
  char *result = strindex(testing0, testing1);
  printf("%s\n", result ? result : "not found");
}
