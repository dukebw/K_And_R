#ifndef ALLOC_H
#define ALLOC_H 
// NOTE(brendan): size of available space
#define ALLOCSIZE 2000000000

// NOTE(brendan): storage for alloc
static char allocbuf[ALLOCSIZE];
// NOTE(brendan): next free position
static char *allocp = allocbuf;

// NOTE(brendan): return pointer to n characters
char *alloc(int n) {
  // NOTE(brendan): it fits
  if(allocbuf + ALLOCSIZE - allocp >= n) {
    allocp += n;
    // NOTE(brendan): old p
    return (allocp - n);
  }
  else {
    // NOTE(brendan): not enough room
    return 0;
  }
}

// NOTE(brendan): free storage pointed to by p
void afree(char *p) {
  if(p >= allocbuf && p < allocbuf + ALLOCSIZE) {
    allocp = p;
  }
}

#endif /* ALLOC_H */
