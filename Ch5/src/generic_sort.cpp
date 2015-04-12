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
#define MAXLINES 4096

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

// NOTE(brendan): reverse of numcmp
int reverseNumcmp(char *s1, char *s2)
{
    return -numcmp(s1, s2);
}

// NOTE(brendan): reverse of strcmp
int reverseStrcmp(char *s1, char *s2)
{
    return -strcmp(s1, s2);
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

// NOTE(brendan): INPUT: a character. OUTPUT: that character
// in lower-case if it was in upper case; the input is output otherwise
char foldCase(char c)
{
    if ((c >= 'A') && (c <= 'Z')) {
        c += 'a' - 'A';
    }
    return c;
}

// NOTE(brendan): compare string cs to string ct;
// return <0 if cs < ct, 0 if cs == ct, or >0 if cs > ct
// treat upper-case and lower-case letters as equal
int foldStrcmp(char *cs, char *ct) {
    while (true) {
        char csTemp = foldCase(*cs);
        char ctTemp = foldCase(*ct);
        if (csTemp < ctTemp) {
            return -1;
        }
        else if (csTemp > ctTemp) {
            return 1;
        }
        // NOTE(brendan): *cs == *ct
        if (*cs == '\0') {
            return 0;
        }
        ++cs;
        ++ct;
    }
    return 0;
}

// TODO(brendan): add the option -f to fold upper and lower case together,
// so that case distinctions are not made during sorting; for example,
// a and A compare equal
// TODO(brendan): foldStrcmp should put capital 'A' before 'a'
// NOTE(brendan): sort input lines
int main(int argc, char *argv[]) {
    // NOTE(brendan): number of input lines read
    int nlines;

    //NOTE(brendan): 1 if numeric sort
    int numeric = 0;
    int reverse = 0;
    int fold = 0;
    for (int argIndex = 1; argIndex < argc; ++argIndex) {
        char *argString = argv[argIndex];
        if (*argString != '-') {
            printf("Error! file input not supported yet\n");
            return 2;
        }
        while (*++argString != '\0') {
            switch (*argString) {
                case 'n':
                {
                    numeric = 1;
                }   break;
                case 'r':
                {
                    reverse = 1;
                }   break;
                case 'f':
                {
                    fold = 1;
                }   break;
                default:
                {
                    printf("Invalid input -- '%c'\n", *argString);
                    return 2;
                }
            }
        }
    }
    if ((nlines = readlines(lineptr, MAXLINES)) >= 0) {
        int (*pNumcmp)(void *, void *) = 
            reverse ? (int (*)(void *, void *))reverseNumcmp : 
                      (int (*)(void *, void *))numcmp;
        int (*pStrcmp)(void *, void *) =
            reverse ? (int (*)(void *, void *))reverseStrcmp :
                      (int (*)(void *, void *))strcmp;
        // TODO(brendan): reverse for fold
        pStrcmp = fold ? (int (*)(void *, void*))foldStrcmp : pStrcmp;
        qsort((void **)lineptr, 0, nlines - 1, (numeric ?  pNumcmp : pStrcmp));
        writelines(lineptr, nlines);
        return 0;
    }
    else {
        printf("input too big to sort\n");
        return 1;
    }
}
