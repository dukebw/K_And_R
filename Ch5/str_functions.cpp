/* ========================================================================
   File: str_functions.cpp
   Date: Feb. 12/15
   Revision: 1
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
  while(*s != '\0') {
    ++s;
  }
  while((*s++ = *t++) != '\0');
  return result;
}

// NOTE(brendan): returns length of string s
int strlen(char *s) {
  char *sStart = s;
  while(*s != '\0') {
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
  for(int i = 0; i < sLength/2; ++i) {
    swap(s + i, s + (sLength - 1) - i);
  }
  return s;
}

// NOTE(brendan): returns true if the string t occurs at the end of the string
// s; false otherwise.
bool strend(char *s, char *t) {
  char *sReverse = reverse(s);
  char *tReverse = reverse(t);
  while(*tReverse != '\0') {
    if(*sReverse != *tReverse) {
      return false;
    }
    ++sReverse;
    ++tReverse;
  }
  return true;
}

// TODO(brendan): strncpy, strncat, and strncmp, which operate on at most
// the first n characters of their argument strings. E.g. strncpy(s,t,n)
// copies at most n characters of t to s. (p. 121)

// TODO(brendan): in progress
// NOTE(brendan): copy at most n characters of string ct to s; return s.
// Pad with '\0's if ct has fewer than n characters.
char *strncpy(char *s, char *ct, int n) {
  int copied = 0;
  while((copied < n) && ((*s = *ct) != '\0'));
}

int main() {
  char testString[] = "1234567890";
  char strendTest[] = "567890";
  printf("%d\n", strend(testString, strendTest));
}
