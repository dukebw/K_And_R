#ifndef ALLOC_H
#define ALLOC_H 

// NOTE(brendan): return pointer to n characters
char *alloc(int n);

// NOTE(brendan): free storage pointed to by p
void afree(char *p);

#endif /* ALLOC_H */
