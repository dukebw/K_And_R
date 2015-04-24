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
    NAME, PARENS, BRACKETS, PRIMITIVETYPE
};

// ----------------------------------------------------------------------------
// Forward declarations
// ----------------------------------------------------------------------------
internal bool32 dirdcl(char *token, int tokentype);

// NOTE(brendan): buffer for ungetch
global_variable char buf[BUFSIZE];
// NOTE(brendan): next free position in buf
global_variable int bufp = 0;
// NOTE(brendan): identifier name
global_variable char name[MAXTOKEN];
// NOTE(brendan): output string
global_variable char out[1000];
global_variable char inbuffer[MAXTOKEN];
global_variable int inbufferindex;

// NOTE(brendan): get a (possibly pushed-back) character
internal int getch(void)
{
#if 0
    return (bufp > 0) ? buf[--bufp] : inbuffer[inbufferindex++];
#else
    return (bufp > 0) ? buf[--bufp] : getchar();
#endif
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

internal bool32
stringequals(const char *s1, const char *s2)
{
    bool32 result = true;
    Stopif(((s1 == 0) || (s2 == 0)),, "Null pointer(s) passed to "
                                      "stringequals");
    for (; (*s1 != 0) || (*s2 != 0); ++s1, ++s2) {
        if (*s1 != *s2) {
            result = false;
            break;
        }
    }
    return result;
}

internal bool32
isprimitivetype(char *s)
{
    bool32 result = false;
    if (stringequals(s, "int") || stringequals(s, "float") ||
        stringequals(s, "char")) {
        result = true;
    }
    return result;
}

// NOTE(brendan): return next token
internal int
gettoken(char *token)
{
    int c;
    char *p = token;
    skipspace(&c);
    if (c == '(') {
        skipspace(&c);
        if (c == ')') {
            strcpy(token, "()");
            return PARENS;
        } else {
            ungetch(c);
            return '(';
        }
    } else if (c == '[') {
        // TODO(brendan): check what is inside the brackets
        for (*p++ = c; (*p++ = getch()) != ']';) {}
        *p = '\0';
        return BRACKETS;
    } else if (isalpha(c)) {
        do {
            *p++ = c;
        } while(isalnum(c = getch()) || (c == '_'));
        *p = '\0';
        ungetch(c);
        if (isprimitivetype(token)) {
            return PRIMITIVETYPE;
        }
        else {
            return NAME;
        }
    } else {
        return c;
    }
}

// TODO(brendan): expand dcl to handle declarations with function argument
// types, qualifiers like const etc.
// NOTE(brendan): dcl: parse a declarator
internal int32 dcl(char *token, int32 tokentype)
{
    // NOTE(brendan): count *'s
    int ns;
    for (ns = 0; (tokentype = gettoken(token)) == '*'; ++ns) {}
    tokentype = dirdcl(token, tokentype);
    while (ns-- > 0) {
        strcat(out, " pointer to");
    }
    return tokentype;
}

// NOTE(brendan): dirdcl: parse a direct declarator
internal int32
dirdcl(char *token, int tokentype)
{
    // NOTE(brendan): ( dcl )
    if (tokentype == '(') {
        tokentype = dcl(token, tokentype);
        if (tokentype != ')') {
            tokentype = -1;
            printf("error: missing )\n");
        }
    } else if (tokentype == NAME) {
        // NOTE(brendan): variable name
        strcpy(name, token);
    } else {
        tokentype = -1;
        printf("error: expected name or (dcl)\n");
    }
    while (((tokentype = gettoken(token)) == PARENS) || (tokentype == BRACKETS)) {
        if (tokentype == PARENS) {
            strcat(out, " function returning");
        } else {
            strcat(out, " array");
            strcat(out, token);
            strcat(out, " of");
        }
    }
    return tokentype;
}

int main(int argc, char **argv)
{
    // NOTE(brendan): data type = char, int, etc.
    char datatype[MAXTOKEN];
    // NOTE(brendan): last token string
    char token[MAXTOKEN];
    // NOTE(brendan): type of last token
    int tokentype;

    FILE *infile = fopen("dcl_parser_test", "r");
    fread(inbuffer, sizeof(char), MAXTOKEN, infile);
    fclose(infile);

    // NOTE(brendan): 'd' argument to go from C -> English.
    // Default: Pseudo-English -> C.
    // E.g. p * [] * () char -> char (*(*p)[])()
    if ((argc > 1) && (stringequals(argv[1], "-d"))) {
        // TODO(brendan): make dcl recover from input errors
        // NOTE(brendan): convert declaration to words
        // NOTE(brendan): 1st token on line is the datatype
        while ((tokentype = gettoken(token)) != EOF) {
            if (tokentype == PRIMITIVETYPE) {
                strcpy(datatype, token);
                // NOTE(brendan): parse rest of line
                tokentype = dcl(token, tokentype);
                if (tokentype != '\n') {
                    printf("syntax error\n");
                }
                if (tokentype != -1) {
                    printf("%s: %s %s\n", name, out, datatype);
                }
                // TODO(brendan): ???
                tokentype = 0;
                out[0] = '\0';
                token[0] = '\0';
            }
        }
    } else {
        // NOTE(brendan): undcl: convert word description to declaration
        char temp[MAXTOKEN];
        while ((tokentype = gettoken(token)) != EOF) {
            strcpy(out, token);
            bool32 gottoken = false;;
            while ((gottoken && (tokentype != '\n')) ||
                   ((tokentype = gettoken(token)) != '\n')) {
                if (gottoken) {
                    gottoken = false;
                }
                if (tokentype == PARENS || tokentype == BRACKETS) {
                    strcat (out, token);
                } else if (tokentype == '*') {
                    tokentype = gettoken(token);
                    gottoken = true;
                    if (tokentype == PARENS || tokentype == BRACKETS) {
                        sprintf(temp, "(*%s)", out);
                    } else {
                        sprintf(temp, "*%s", out);
                    }
                    strcpy(out, temp);
                } else if ((tokentype == NAME) ||
                           (tokentype == PRIMITIVETYPE)) {
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
