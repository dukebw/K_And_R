/* ========================================================================
   File: util.cpp
   Date: Mar. 2/2015
   Revision: 1
   Creator: Brendan Duke
   Notice: (C) Copyright 2015 by BRD Inc. All Rights Reserved.
   ======================================================================== */

#include "util.h"
#include "alloc.h"
#include <stdio.h>
#include <string.h>

// NOTE(brendan): max length of any input line
#define MAXLEN 10000

// NOTE(brendan): read a line into s; return length
int GetLine(char s[], int lim) {
    int c;

    char *start = s;
    while ((lim-- > 0) && ((c=getchar()) != EOF) && (c != '\n')) {
        *s++ = c;
    }
    if (c == '\n') {
        *s++ = c;
    }
    *s = '\0';
    return (s - start);
}

//NOTE(brendan): read input lines
int ReadLines(char *lineptr[], int maxlines) {
    int len, nlines = 0;
    char *p, line[MAXLEN];

    while ((len = GetLine(line, MAXLEN)) > 0) {
        if (nlines >= maxlines || (p = alloc(len)) == NULL) {
            return -1;
        }
        else {
            // NOTE(brendan): delete newline
            line[len - 1] = '\0';
            strcpy(p, line);
            lineptr[nlines++] = p;
        }
    }
    return nlines;
}

// NOTE(brendan): write output lines
void WriteLines(char *lineptr[], int nlines) {
    while (nlines-- > 0) {
        printf("%s\n", *lineptr++);
    }
}

// NOTE(brendan): interchange v[i] and v[j]
void Swap(void *v[], int i, int j) {
    void *temp;

    temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}
