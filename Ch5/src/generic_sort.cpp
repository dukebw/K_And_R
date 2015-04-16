/* ========================================================================
   File: generic_sort.cpp
   Date: Mar. 5/15
   Revision: 1
   Creator: Brendan Duke
   Notice: (C) Copyright 2015 by BRD Inc. All Rights Reserved.
   ======================================================================== */

#include "util.h" // NOTE(brendan): Swap, ReadLines, WriteLines

// NOTE(brendan): max #lines to be sorted
#define MAXLINES 4096
#define COMPARE_CHAR_FUNCTION(name) int name(char *, char *)
typedef COMPARE_CHAR_FUNCTION(compare_char_fn);
#define COMPARE_VOID_FUNCTION(name) int name(void *, void *)
typedef COMPARE_VOID_FUNCTION(compare_void_fn);

// NOTE(brendan): pointers to text lines
global_variable char *GlobalLinePointer[MAXLINES];
global_variable compare_char_fn *GlobalSortFunction = (compare_char_fn *)strcmp;
// TODO(brendan): make global_options struct?
global_variable int GlobalReverse = 0;

// NOTE(brendan): compare s1 and s2 numerically
internal int
NumComp(char *StringA, char *StringB)
{
    double ValueA = atof(StringA);
    double ValueB = atof(StringB);
    if (ValueA < ValueB) {
        return -1;
    }
    else if (ValueA > ValueB) {
        return 1;
    }
    else {
        return 0;
    }
}

// NOTE(brendan): increment char pointer past non-space/alpha-numeric
inline void
MoveToAlphaNumeric(char **s)
{
    while (!(isspace(**s) || isalnum(**s)) &&
           (**s != '\0')) {
        ++*s;
    }
}

// NOTE(brendan): reverse of GlobalSortFunction
internal int
ReverseFunction(char *StringA, char *StringB)
{
    return -GlobalSortFunction(StringA, StringB);
}

// NOTE(brendan): use directory-order version of GlobalSortFunction
internal int
DirectorySortFunction(char *StringA, char *StringB)
{
    MoveToAlphaNumeric(&StringA);
    MoveToAlphaNumeric(&StringB);
    return GlobalReverse ? -GlobalSortFunction(StringA, StringB) :
                            GlobalSortFunction(StringA, StringB);
}

// NOTE(brendan): sort v[left]..v[right] into increasing order
internal void
QuickSort(void *v[], int Left, int Right, compare_void_fn *Comp)
{
    void Swap(void *v[], int, int); 

    // NOTE(brendan): do nothing if array contains < 2 elements
    if (Left >= Right) {
        return;
    }
    Swap(v, Left, (Left + Right)/2);
    int Last = Left;
    for (int Index = Left + 1; Index <= Right; ++Index) {
        if ((*Comp)(v[Index], v[Left]) < 0) {
            Swap(v, ++Last, Index);
        }
    }
    Swap(v, Left, Last);
    QuickSort(v, Left, Last - 1, Comp);
    QuickSort(v, Last + 1, Right, Comp);
}

// NOTE(brendan): INPUT: a character. OUTPUT: that character
// in lower-case if it was in upper case; the input is output otherwise
internal char
FoldCase(char InputChar)
{
    if ((InputChar >= 'A') && (InputChar <= 'Z')) {
        InputChar += 'a' - 'A';
    }
    return InputChar;
}

// NOTE(brendan): compare string StringA to string StringB;
// return < 0 if StringA < StringB, 0 if StringA == StringB,
// or > 0 if StringA > StringB
// treat upper-case and lower-case letters as equal
internal int
FoldStrComp(char *StringA, char *StringB)
{
    while (true) {
        char StringATemp = FoldCase(*StringA);
        char StringBTemp = FoldCase(*StringB);
        if (StringATemp < StringBTemp) {
            return -1;
        }
        else if (StringATemp > StringBTemp) {
            return 1;
        }
        else if (*StringA > *StringB) {
            return 1;
        }
        else if (*StringA < *StringB) {
            return -1;
        }
        // NOTE(brendan): *StringA == *StringB
        if (*StringA == '\0') {
            return 0;
        }
        ++StringA;
        ++StringB;
    }
    return 0;
}

// TODO(brendan): Add a field-handling capability, so sorting may be done
// on the fields within lines, each field sorted according to an independent
// set of options. (E.g. index of book: -df for index category and -n
// for page numbers)
// NOTE(brendan): sort input lines
int main(int argc, char *argv[])
{
    compare_char_fn *SortFunction = GlobalSortFunction;

    // NOTE(brendan): number of input lines read
    int LineCount;

    // NOTE(brendan): whether to sort numerically or fold
    int Numeric = 0;
    int Fold = 0;
    int Directory = 0;
    for (int ArgIndex = 1; ArgIndex < argc; ++ArgIndex) {
        char *ArgString = argv[ArgIndex];
        if (*ArgString != '-') {
            printf("Error! file input not supported yet\n");
            return 2;
        }
        while (*++ArgString != '\0') {
            switch (*ArgString) {
                case 'n':
                {
                    Numeric = 1;
                }   break;
                case 'r':
                {
                    GlobalReverse = 1;
                }   break;
                case 'f':
                {
                    Fold = 1;
                }   break;
                case 'd':
                {
                    Directory = 1;
                }   break;
                default:
                {
                    printf("Invalid input -- '%c'\n", *ArgString);
                    return 2;
                }
            }
        }
    }
    if (Numeric) {
        GlobalSortFunction = NumComp;
    }
    else if (Fold) {
        GlobalSortFunction = FoldStrComp;
    }
    if (GlobalReverse) {
        SortFunction = ReverseFunction;
    }
    if (Directory) {
        SortFunction = DirectorySortFunction;
    }
    if ((LineCount = ReadLines(GlobalLinePointer, MAXLINES)) >= 0) {
        QuickSort((void **)GlobalLinePointer, 0, LineCount - 1,
                  (compare_void_fn *)SortFunction);
        WriteLines(GlobalLinePointer, LineCount);
        return 0;
    }
    else {
        printf("input too big to sort\n");
        return 1;
    }
}
