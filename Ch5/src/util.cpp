/* ========================================================================
   File: util.cpp
   Date: Mar. 2/2015
   Revision: 1
   Creator: Brendan Duke
   Notice: (C) Copyright 2015 by BRD Inc. All Rights Reserved.
   ======================================================================== */

#include "util.h"
#include <stdio.h>

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
