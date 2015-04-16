#ifndef UTIL_H
#define UTIL_H

/* #include "stopif.h" */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#define internal static
#define local_persist static
#define global_variable static

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef int32 bool32;

typedef float real32;
typedef double real64;

// NOTE(brendan): read a line into s; return length
int GetLine(char s[], int lim);

//NOTE(brendan): read input lines
int ReadLines(char *lineptr[], int maxlines);

// NOTE(brendan): write output lines
void WriteLines(char *lineptr[], int nlines);

// NOTE(brendan): interchange v[i] and v[j]
void Swap(void *v[], int i, int j);

#endif /* UTIL_H */
