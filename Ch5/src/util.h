#ifndef UTIL_H
#define UTIL_H

// NOTE(brendan): read a line into s; return length
int getline(char s[], int lim);

//NOTE(brendan): read input lines
int readlines(char *lineptr[], int maxlines);

// NOTE(brendan): write output lines
void writelines(char *lineptr[], int nlines);

// NOTE(brendan): interchange v[i] and v[j]
void swap(void *v[], int i, int j);

#endif /* UTIL_H */
