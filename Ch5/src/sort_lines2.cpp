/* ========================================================================
   File: sort_lines2.cpp
   Date: Feb. 18/15
   Revision: 1
   Creator: Brendan Duke
   Notice: (C) Copyright 2015 by BRD Inc. All Rights Reserved.
   ======================================================================== */

#include <stdio.h>
#include <string.h>
#include "alloc.h"

// NOTE(brendan): max #lines to be sorted
#define MAXLINES 5000
// NOTE(brendan): max length of any input line
#define MAXLEN 10000
// NOTE(brendan): max number of chars in file to be sorted
#define MAXCHAR 200000

// NOTE(brendan): pointers to text lines
char *lineptr[MAXLINES];

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

char *alloc(int);

//NOTE(brendan): read input lines
int readlines(char *lineptr[], char lineStorage[], int maxlines) {
  int len, nlines = 0;
  char line[MAXLEN];

  while((len = getline(line, MAXLEN)) > 0) {
    if(nlines >= maxlines) {
      return -1;
    }
    else {
      // NOTE(brendan): delete newline
      line[len - 1] = '\0';
      strcpy(lineStorage, line);
      lineptr[nlines++] = lineStorage;
      lineStorage += len;
    }
  }
  return nlines;
}

// NOTE(brendan): write output lines
void writelines(char *lineptr[], int nlines) {
  while(nlines-- > 0) {
    printf("%s\n", *lineptr++);
  }
}

// NOTE(brendan): interchange v[i] and v[j]
void swap(char *v[], int i, int j) {
  char *temp;

  temp = v[i];
  v[i] = v[j];
  v[j] = temp;
}

// NOTE(brendan): sort v[left...right] into increasing order
void qsort(char *v[], int left, int right) {
  // NOTE(brendan): do nothing if array contains fewer than two elements
  if(left >= right) {
    return;
  }
  swap(v, left, (left + right)/2);
  int last = left;
  for(int i = left + 1; i <= right; ++i) {
    if(strcmp(v[i], v[left]) < 0) {
      swap(v, ++last, i);
    }
  }
  swap(v, left, last);
  qsort(v, left, last - 1);
  qsort(v, last + 1, right);
}

// NOTE(brendan): sort input lines
int main(int argc, char *argv[]) {
  // NOTE(brendan): storage for lines to be pass to readlines
  char lineStorage[MAXCHAR];
  // NOTE(brendan): number of input lines read
  int nlines;
  if((nlines = readlines(lineptr, lineStorage, MAXLINES)) >= 0) {
    qsort(lineptr, 0, nlines - 1);
    writelines(lineptr, nlines);
    return 0;
  }
  else {
    printf("error: input too big to sort\n");
    return 1;
  }
}
