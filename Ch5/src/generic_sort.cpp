/* ========================================================================
File: generic_sort.cpp
Date: Mar. 5/15
Revision: 1
Creator: Brendan Duke
Notice: (C) Copyright 2015 by BRD Inc. All Rights Reserved.
======================================================================== */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h" // NOTE(brendan): swap, readlines, writelines

// NOTE(brendan): max #lines to be sorted
#define MAXLINES 5000

// NOTE(brendan): pointers to text lines
char *lineptr[MAXLINES];

int readlines(char *lineptr[], int nlines);
void writelines(char *lineptr[], int nlines);

// NOTE(brendan): compare s1 and s2 numerically
int numcmp(char *s1, char *s2) {
    double v1 = atof(s1);
    double v2 = atof(s2);
    if (v1 < v2) {
        return -1;
    }
    else if (v1 > v2) {
        return 1;
    }
    else {
        return 0;
    }
}

// NOTE(brendan): sort v[left]..v[right] into increasing order
void qsort(void *v[], int left, int right, int (*comp)(void *, void *)) {
    void swap(void *v[], int, int); 

    // NOTE(brendan): do nothing if array contains < 2 elements
    if (left >= right) {
        return;
    }
    swap(v, left, (left + right)/2);
    int last = left;
    for (int i = left + 1; i <= right; ++i) {
        if ((*comp)(v[i], v[left]) < 0) {
            swap(v, ++last, i);
        }
    }
    swap(v, left, last);
    qsort(v, left, last - 1, comp);
    qsort(v, last + 1, right, comp);
}

// TODO(brendan): modify program to handle a -r flag, which indicates
// sorting in reverse (decreasing) order. Be sure that -r works with -n.
// NOTE(brendan): sort input lines
int main(int argc, char *argv[]) {
    // NOTE(brendan): number of input lines read
    int nlines;

    //NOTE(brendan): 1 if numeric sort
    int numeric = 0;

    if ((argc > 1) && (strcmp(argv[1], "-n") == 0)) {
        numeric = 1;
    }
    if ((nlines = readlines(lineptr, MAXLINES)) >= 0) {
        qsort((void **)lineptr, 0, nlines - 1, 
              (numeric ? 
               (int (*)(void *, void *))numcmp : 
               (int (*)(void *, void *))strcmp));
        writelines(lineptr, nlines);
        return 0;
    }
    else {
        printf("input too big to sort\n");
        return 1;
    }
}
