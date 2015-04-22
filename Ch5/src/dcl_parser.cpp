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
internal bool32 dirdcl();

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
    } else {
        buf[bufp++] = c;
    }
}

// NOTE(brendan): INPUT: none. OUTPUT: first char in input stream (stdin?)
// that is not a space or tab
inline void
skipspace(int *c)
{
    while (((*c = getch()) == ' ') || (*c == '\t')) {}
}

// NOTE(brendan): return next token
internal int
gettoken()
{
    int c;
    char *p = token;
    skipspace(&c);
    if (c == '(') {
        skipspace(&c);
        if (c == ')') {
            strcpy(token, "()");
            return (tokentype = PARENS);
        } else {
            ungetch(c);
            return (tokentype = '(');
        }
    } else if (c == '[') {
        // TODO(brendan): check what is inside the brackets
        for (*p++ = c; (*p++ = getch()) != ']';) {}
        *p = '\0';
        return (tokentype = BRACKETS);
    } else if (isalpha(c)) {
        do {
            *p++ = c;
        } while(isalnum(c = getch()) || (c == '_'));
        *p = '\0';
        ungetch(c);
        return (tokentype = NAME);
    } else {
        return (tokentype = c);
    }
}

// NOTE(brendan): dcl: parse a declarator
internal bool32 dcl()
{
    // NOTE(brendan): count *'s
    int ns;
    for (ns = 0; gettoken() == '*'; ++ns) {}
    bool32 success = dirdcl();
    while (ns-- > 0) {
        strcat(out, " pointer to");
    }
    return success;
}

// NOTE(brendan): dirdcl: parse a direct declarator
internal bool32
dirdcl()
{
    bool32 success = true;
    // NOTE(brendan): ( dcl )
    if (tokentype == '(') {
        success = dcl();
        if (tokentype != ')') {
            success = false;
            printf("error: missing )\n");
        }
    } else if (tokentype == NAME) {
        // NOTE(brendan): variable name
        strcpy(name, token);
    } else {
        success = false;
        printf("error: expected name or (dcl)\n");
    }
    int type;
    while (((type = gettoken()) == PARENS) || (type == BRACKETS)) {
        if (type == PARENS) {
            strcat(out, " function returning");
        } else {
            strcat(out, " array");
            strcat(out, token);
            strcat(out, " of");
        }
    }
    return success;
}

int main(int argc, char **argv)
{
    if ((argc > 1) && (tolower(*argv[1]) == 'd')) {
        // TODO(brendan): make dcl recover from input errors
        // NOTE(brendan): convert declaration to words
        // NOTE(brendan): 1st token on line is the datatype
        while (gettoken() != EOF) {
            strcpy(datatype, token);
            // NOTE(brendan): parse rest of line
            bool32 success = dcl();
            if (tokentype != '\n') {
                printf("syntax error\n");
            }
            if (success) {
                printf("%s: %s %s\n", name, out, datatype);
            }
            // TODO(brendan): ???
            tokentype = 0;
            out[0] = '\0';
            token[0] = '\0';
        }
    } else {
        // NOTE(brendan): undcl: convert word description to declaration
        int type;
        char temp[MAXTOKEN];
        while (gettoken() != EOF) {
            strcpy(out, token);
            bool32 gottoken = false;;
            while ((gottoken && type != '\n') ||
                   ((type = gettoken()) != '\n')) {
                if (gottoken) {
                    gottoken = false;
                }
                if (type == PARENS || type == BRACKETS) {
                    strcat (out, token);
                } else if (type == '*') {
                    type = gettoken();
                    gottoken = true;
                    if (type == PARENS || type == BRACKETS) {
                        sprintf(temp, "(*%s)", out);
                    } else {
                        sprintf(temp, "*%s", out);
                    }
                    strcpy(out, temp);
                } else if (type == NAME) {
                    sprintf(temp, "%s %s", token, out);
                    strcpy(out, temp);
                } else {
                    printf("invalid input at %s\n", token);
                }
            }
            printf("%s\n", out);
        }
    }
}
