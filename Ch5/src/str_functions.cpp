/* ========================================================================
   File: str_functions.cpp
   Date: Feb. 16/15
   Revision: 2
   Creator: Brendan Duke
   Notice: (C) Copyright 2015 by BRD Inc. All Rights Reserved.
   This is a file containing various common C library functions on strings,
   implemented using pointers.
   ======================================================================== */

#include <stdio.h>

#define MAXLINE 128

// NOTE(brendan): copies the string t to the end of s
char *strcat(char *s, char *t) {
    char *result = s;
    while (*s != '\0') {
        ++s;
    }
    while ((*s++ = *t++) != '\0');
    return result;
}

// NOTE(brendan): returns length of string s
int strlen(char *s) {
    char *sStart = s;
    while (*s != '\0') {
        ++s;
    }
    return ((s - sStart) / sizeof(char));
}

// NOTE(brendan): swap the characters s and t (xor trick)
void swap(char *s, char *t) {
    *s ^= *t;
    *t ^= *s;
    *s ^= *t;
}

// NOTE(brendan): reverses string s and returns pointer to start of s;
// side-effects
char *reverse(char *s) {
    int sLength = strlen(s);
    for (int i = 0; i < sLength/2; ++i) {
        swap(s + i, s + (sLength - 1) - i);
    }
    return s;
}

// NOTE(brendan): returns true if the string t occurs at the end of the string
// s; false otherwise.
bool strend(char *s, char *t) {
    char *sReverse = reverse(s);
    char *tReverse = reverse(t);
    while (*tReverse != '\0') {
        if (*sReverse != *tReverse) {
            return false;
        }
        ++sReverse;
        ++tReverse;
    }
    return true;
}

// NOTE(brendan): copy at most n characters of string ct to s; return s.
// Pad with '\0's if ct has fewer than n characters.
char *strncpy(char *s, char *ct, int n) {
    char *result = s;
    while (((s - result) < n) && ((*s++ = *ct++) != '\0'));
    while ((s - result) < n) {
        *s++ = '\0';
    }
    return result;
}

// NOTE(brendan): concatenate at most n characters of string ct to string s;
// terminate s with '\0'; return s.
char *strncat(char *s, char *ct, int n) {
    char *result = s;
    while (*s != '\0') {
        ++s;
    }
    char *start = s;
    while (((s - start) < n) && ((*s++ = *ct++) != '\0'));
    while ((s - start) < n) {
        *s++ = '\0';
    }
    return result;
}

// NOTE(brendan): compare at most n characters of string cs to string ct;
// return <0 if cs < ct, 0 if cs == ct, or >0 if cs > ct
int strncmp(char *cs, char *ct, int n) {
    for (int i = 0; i < n; ++i) {
        if (*cs < *ct) {
            return -1;
        }
        else if (*cs > *ct) {
            return 1;
        }
        if (*cs == '\0') {
            return 0;
        }
        // NOTE(brendan): *cs == *ct
        ++cs;
        ++ct;
    }
    return 0;
}

int main() {
    char testString0[MAXLINE] = "abcdeFghijklmnopqrstuvwxyz";
    char testString1[] = "abcdefghijklmnopqrstuvwxyz";
    char strendTest[] = "567890";
    printf("%d\n", strncmp(testString0, testString1, 6));
}
