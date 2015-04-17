/* ========================================================================
   File: dcl_parser.cpp
   Date: Apr. 17/15
   Revision: 1
   Creator: Brendan Duke
   Notice: (C) Copyright 2015 by BRD Inc. All Rights Reserved.
   ======================================================================== */

#include "util.h"

#define MAXTOKEN 100
#define BUFSIZE 100

enum {
    NAME, PARENS, BRACKETS
};

// ----------------------------------------------------------------------------
// Forward declarations
// ----------------------------------------------------------------------------
internal void dirdcl();

// NOTE(brendan): buffer for ungetch
global_variable char buf[BUFSIZE];
// NOTE(brendan): next free position in buf
global_variable int bufp = 0;
// NOTE(brendan): type of last token
global_variable int tokentype;
// NOTE(brendan): last token string
global_variable char token[MAXTOKEN];
// NOTE(brendan): identifier name
global_variable char name[MAXTOKEN];
// NOTE(brendan): data type = char, int, etc.
global_variable char datatype[MAXTOKEN];
// NOTE(brendan): output string
global_variable char out[1000];

// NOTE(brendan): get a (possibly pushed-back) character
internal int getch(void)
{
    return (bufp > 0) ? buf[--bufp] : getchar();
}

// NOTE(brendan): push character back on input
void ungetch(int c)
{
    if (bufp >= BUFSIZE) {
        printf("ungetch: too many character\n");
    }
    else {
        buf[bufp++] = c;
    }
}

// NOTE(brendan): return next token
internal int gettoken()
{
    int c;
    char *p = token;
    while (((c = getch()) == ' ') || (c == '\t'));
    if (c == '(') {
        if ((c = getch()) == ')') {
            strcpy(token, "()");
            return (tokentype = PARENS);
        }
        else {
            ungetch(c);
            return (tokentype = '(');
        }
    }
    else if (c == '[') {
        for (*p++ = c; (*p++ = getch()) != ']';);
        *p = '\0';
        return (tokentype = BRACKETS);
    }
    else if (isalpha(c)) {
        for (*p++ = c; isalnum(c = getch()); *p++ = c);
        *p = '\0';
        ungetch(c);
        return (tokentype = NAME);
    }
    else {
        return (tokentype = c);
    }
}

// NOTE(brendan): dcl: parse a declarator
internal void dcl()
{
    // NOTE(brendan): count *'s
    int ns;
    for (ns = 0; gettoken() == '*'; ++ns);
    dirdcl();
    while (ns-- > 0) {
        strcat(out, " pointer to");
    }
}

// NOTE(brendan): dirdcl: parse a direct declarator
internal void dirdcl()
{
    // NOTE(brendan): ( dcl )
    if (tokentype == '(') {
        dcl();
        if (tokentype != ')') {
            printf("error: missing )\n");
        }
    }
    // NOTE(brendan): variable name
    else if (tokentype == NAME) {
        strcpy(name, token);
    }
    else {
        printf("error: expected name or (dcl)\n");
    }
    int type;
    while ((type = gettoken()) == PARENS || type == BRACKETS) {
        if (type == PARENS) {
            strcat(out, " function returning");
        }
        else {
            strcat(out, " array");
            strcat(out, token);
            strcat(out, " of");
        }
    }
}

// NOTE(brendan): convert declaration to words
int main(int argc, char **argv)
{
    // NOTE(brendan): 1st token on line is the datatype
    while (gettoken() != EOF) {
        strcpy(datatype, token);
        out[0] = '\0';
        // NOTE(brendan): parse rest of line
        dcl();
        if (tokentype != '\n') {
            printf("syntax error\n");
        }
        printf("%s: %s %s\n", name, out, datatype);
    }
    return 0;
}
