#include <stdio.h>
#include <ctype.h>
/* load i/o routines */
/* load character test routines */
#define BSIZE 128 /* buffer size */
#define NONE -1
#define EOS '\0'

#define NUM 256
#define DIV 257
#define MOD 258
#define ID 259
#define DONE 260

int tokenval; /* value of token attribute */
int lineno;
struct entry { /* form of symbol table entry */
    char *lexptr;
    int token;
};
struct entry symtable[];  /*  symbol table */
