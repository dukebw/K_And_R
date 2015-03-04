/* ========================================================================
   File: tail.cpp
   Date: Mar. 4/15
   Revision: 1
   Creator: Brendan Duke
   Notice: (C) Copyright 2015 by BRD Inc. All Rights Reserved.
   Prints the last n lines of its input. By default, n is 10, but it can be
   provided as an argument as in "tail -n".
   ======================================================================== */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "util.h"
#include "alloc.h"

// NOTE(brendan): max number of lines to be displayed (max n)
#define MAXLINE 5000
#define MAXLEN 10000
#define N_DEFAULT 10

int main(int argc, char *argv[]) {
  char *nLastLines[MAXLINE];
  int n = N_DEFAULT;
  
  int c;
  while((--argc > 0) && ((*++argv)[0] == '-')) {
    c = *++argv[0];
    if(isdigit(c)) {
      n = c - '0';
      while(isdigit(c = *++argv[0])) {
        n = 10*n + (c - '0');
      }
    }
    else {
      printf("tail: illegal option %c\n", c);
      argc = 0;
    }
  }

  int len, lineCount = 0;;
  char *p, line[MAXLEN];
  while((len = getline(line, MAXLEN)) > 0) {
    p = alloc(len);
    line[len - 1] = '\0';
    strcpy(p, line);
    nLastLines[lineCount++ % n] = p;
  }
  // NOTE(brendan): cases: lineCount > n, lineCount <= n
  // lines wrap/don't wrap
  int tailEndLine = (lineCount) % n;
  if(lineCount > n) {
    for(int i = tailEndLine; i < n; ++i) {
      printf("%s\n", nLastLines[i]);
    }
  }
  for(int i = 0; i < tailEndLine; ++i) {
    printf("%s\n", nLastLines[i]);
  }
}
